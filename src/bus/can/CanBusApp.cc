#include "CanBusApp.h"

void CanBusApp::initialize() {
    numSent = 0;
    numErr = 0;
    WATCH(numSent);
    WATCH(numErr);

    busytime = 0.0;
    busytimestamp = 0.0;
    errpos = INT_MAX;
    errored = false;
    stateok = true;
    errorcount = 0;
    ack_rcvd = true;
    idle = true;
    char buf[64];
    sprintf(buf, "state: idle");
    bubble("state: idle");
    getDisplayString().setTagArg("tt", 0, buf);
    payload = false;
    errors = true;
    ack = false;
    bandwidth = 1000000;
    payload = getParentModule()->par("payload");
    errors = getParentModule()->par("errors");
    ack = getParentModule()->par("ack");
    bandwidth = getParentModule()->par("bandwidth");
    int nodecount = getParentModule()->par("nodecount");
    int i;
    busPort *port = (busPort*) (getParentModule()->getSubmodule("busPort"));
    for (i = 0; i < nodecount; i++) {
        ArbMsg *init = new ArbMsg("initialize");
        init->setNode(i);
        port->sendMsgToNode(init, i);
    }

}

void CanBusApp::finish() {
    simtime_t busload = (busytime / simTime()) * 100;
    if (busload == 0.0 && !idle) {
        busload = 100.0;
    }
    recordScalar("#Simulated_Time", simTime());
    recordScalar("%Busload", busload);
    recordScalar("#Data-Frames", numSent);
    recordScalar("#Errors", numErr);
    double errpercentage = (numErr / (double) numSent) * 100;
    recordScalar("%Errors", errpercentage);
}

void CanBusApp::signIn(cMessage *msg) {
//    ArbMsg *am = check_and_cast<ArbMsg *>(msg);
//    ids.push_back(
//            new CanID(am->getId(), am->getNode(), simTime(), am->getRtr(),
//                    am->getRemotesent()));
    if (idle) {
        cMessage *self = new cMessage("idle_signin");
        ack_rcvd = true;
        scheduleAt(simTime() + (1 / (double) bandwidth), self);
        idle = false;
        busytimestamp = simTime();
        char buf[64];
        sprintf(buf, "state: busy");
        bubble("state: busy");
        getDisplayString().setTagArg("tt", 0, buf);
    }
    delete msg;
}

void CanBusApp::signOut(cMessage *msg) {
    ArbMsg *am = check_and_cast<ArbMsg *>(msg);
    int delid = am->getId();
    bool delrtr = am->getRtr();
    for (list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) {
        CanID *id = *it;
        if (id->getId() == delid && id->getRtr() == delrtr) {
            it = ids.erase(it);
        }
    }
    EV<< "Check-Out fuer die ID " << am->getId() << endl;
    delete msg;
}

void CanBusApp::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) { //Bus ist wieder im Idle-Zustand
        busPort *port = (busPort*) (getParentModule()->getSubmodule("busPort"));
        string name = msg->getName();

        if (name.compare("idle") == 0) { //Wenn zuvor eine Nachricht gesendet wurde

            if (errorcount == 0 || !errors) {
                ArbMsg *am = check_and_cast<ArbMsg *>(msg);

                if (ack) {
                    checkAcknowledgementReception(am);
                } else {
//                    ArbMsg *newam = new ArbMsg("SendingComplete");
//                    newam->setId(am->getId());
                    //TODO aaahhhhhhhh
//                    port->sendMsgToNode(newam, am->getNode());
                    OutputBuffer* controller = check_and_cast<OutputBuffer*>(sendingNode);
                    controller->sendingCompleted(currentSendingID);
                    stateok = true;
                }

            } else {
                errorcount--;
            }

        } else if (name.compare("SendingComplete") == 0) {
            ArbMsg *am = check_and_cast<ArbMsg *>(msg);
            AckMsg *newam = new AckMsg("SendingComplete");
            newam->setId(am->getId());
            stateok = true;
            port->sendMsgToNode(newam, am->getNode());

        } else if (name.compare("ErrResend") == 0) {
            ArbMsg *ef = check_and_cast<ArbMsg *>(msg);
            ArbMsg *sp = new ArbMsg("ErrResendCompl");
            sp->setRemotesent(true);
            sp->setId(ef->getId());
            sp->setNode(ef->getNode());
            scheduleAt(simTime() + (errpos / (double) bandwidth), sp);
            errored = false;
            stateok = false;
            errpos = INT_MAX;

        } else if (name.compare("ErrResendCompl") == 0) {
            ArbMsg *ef = check_and_cast<ArbMsg *>(msg);
            ArbMsg *sp = new ArbMsg("SendingPermission");
            sp->setSignInTime(currsit);
            sp->setRemotesent(true);
            sp->setId(ef->getId());
            vector<list<CanID*>::iterator> eraseids;
            for (list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) {
                CanID *id = *it;
                if (id->getId() == ef->getId()) {
                    eraseids.push_back(it);
                }
            }
            for (unsigned int it = 0; it != eraseids.size(); it++) {
                ids.erase(eraseids.at(it));
            }
            port->sendMsgToNode(sp, ef->getNode());
        }

        if ((!errors || errorcount == 0) && ((ack && ack_rcvd) || !ack)
                && stateok) { //Auswahl der nächsten zu sendenden Nachricht
            grantSendingPermission();
        }

        ack_rcvd = true;
        delete msg;

    } else { // externe Nachricht
        string name = msg->getName();
        if (name.compare("ArbIn") == 0) {
            signIn(msg);
        } else if (name.compare("ArbOut") == 0) {
            signOut(msg);
        } else if (name.compare("ack") == 0) {
            AckMsg *ackm = check_and_cast<AckMsg *>(msg);
            if (ackm->getAck()) {
                ack_rcvd = true;
                //EV << "Acknowledged" << endl;
            }
            delete msg;
        } else if (name.compare("senderror") == 0
                || name.compare("geterror") == 0) {
            handleErrorFrame(msg);
        } else { //Es handelt sich um einen Data-Frame
            handleDataFrame(msg);
        }
    }
}

void CanBusApp::checkAcknowledgementReception(ArbMsg *am) {
    if (ack_rcvd) {
        EV<< "Nachricht bestaetigt" << endl;
        ArbMsg *newam = new ArbMsg("SendingComplete");
        newam->setNode(am->getNode());
        newam->setId(am->getId());
        scheduleAt(simTime() + (11 / (double) bandwidth),
                newam);
        stateok = false;
    } else {
        EV << "Nachricht nicht bestaetigt" << endl;
        AckMsg *newam = new AckMsg("SendingFailed");
        newam->setAck(false);
        newam->setId(am->getId());
        busPort *port = (busPort*) (getParentModule()->getSubmodule("busPort"));
        port->sendMsgToNode(newam, am->getNode());
        ArbMsg *sp = new ArbMsg("SendingPermission");
        sp->setSignInTime(currsit);
        sp->setId(am->getId());
        sp->setRemotesent(true);
        port->sendMsgToNode(sp, am->getNode());
    }
    delete am;
}

void CanBusApp::grantSendingPermission() {
    currentSendingID = INT_MAX;
//    int highestprio = INT_MAX;
//    CanNodeController *hprionode = NULL;
    sendingNode = NULL;

//    bool remotes = false;
    list<CanID*>::iterator delit;
    vector<list<CanID*>::iterator> eraseids;
    for (list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) { //finden der höchsten Priorität aller angemeldeten Nachrichten
        CanID *id = *it;
        if (id->getId() < currentSendingID) {
            currentSendingID = id->getId();
//            if (id->getId() < highestprio) {
//                highestprio = id->getId();
            sendingNode = (OutputBuffer*) id->getNode();
//            hprionode = (CanNodeController*) id->getNode();
//            remotes = id->getRemotesent();
            currsit = id->getSignInTime();
        }
    }
    int sendcount = 0;
    for (list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) { //finden, ob remote frame für diese ID auch gesendet werden soll und das löschen aller frames mit dieser ID wird eingeleitet
        CanID *id = *it;
        if (id->getId() == currentSendingID) {
//            if (id->getId() == highestprio) {
            if (id->getRtr() == false) { //Data-Frame
                if (sendingNode != id->getNode()) { //bei dem ursprünglich gefundenen node handelt es sich um einen remote frame
//                    if (hprionode != id->getNode()) {
                    sendingNode = (OutputBuffer*) id->getNode(); //der Node, der einen Data frame senden möcte wird zum senden ausgewählt
//                    hprionode = (CanNodeController*) id->getNode();
//                    remotes = id->getRemotesent();
                    currsit = id->getSignInTime();
                    sendcount++;
                }
                eraseids.push_back(it);
            } else { //Remote-Frame
                eraseids.push_back(it);
            }
        }
    }
    if (sendcount > 1) {
        EV<< "Mehr als ein Knoten will mit der selben ID senden\n";
        EV<< "Da dies zu fortlaufenden Bit-Fehlern durch die versendenden Knoten fuehrt, wird die Simulation angehalten\n";
        endSimulation();
    }
    if (sendingNode != NULL) {
//        if (hprionode != NULL) {
        for (unsigned int it = 0; it != eraseids.size(); it++) {
            ids.erase(eraseids.at(it));
        }
//        ArbMsg *am = new ArbMsg("SendingPermission");
//        am->setSignInTime(currsit);
//        am->setId(highestprio);
//        if (remotes) {
//            am->setRemotesent(true);
//        } else {
//            am->setRemotesent(false);
//        }
//        busPort *port = (busPort*) (getParentModule()->getSubmodule("busPort"));
//        port->sendMsgToNode(am, hprionode); //Erlaubnis verschicken, ID in Liste bereits gel�scht
        OutputBuffer* controller = check_and_cast<OutputBuffer *>(sendingNode);
        controller->receiveSendingPermission(currentSendingID);
//        hprionode->receiveSendingPermission(highestprio);
    } else {
        EV<< "no pending message" << endl;
        simtime_t timetaken = simTime() - busytimestamp;
        busytime += timetaken;
        idle = true;
        char buf[64];
        sprintf(buf, "state: idle");
        getDisplayString().setTagArg("tt", 0, buf);
        bubble("state: idle");
    }
}

void CanBusApp::handleErrorFrame(cMessage *msg) {
    ErrorFrame *ef = check_and_cast<ErrorFrame *>(msg);
    if (!errored) {
        ArbMsg *amrs = new ArbMsg("ErrResend");
        amrs->setId(ef->getId());
        amrs->setNode(ef->getNode());
        scheduleAt(simTime() + (12 / (double) bandwidth), amrs); //12 - maximale L�nge eines Error-Frames
        errorcount++;
        numErr++;
        errored = true;
    }
    if (errpos > ef->getPos()) {
        if (errpos < INT_MAX) {
            errpos = ef->getPos(); //Position wird ge�ndert, aber nicht erneut an die Knoten gemeldet
            delete msg;
        } else {
            errpos = ef->getPos();
            busPort *port = (busPort*) (getParentModule()->getSubmodule(
                    "busPort"));
            port->forward_to_all(msg);
        }
    } else {
        delete msg;
    }
}

void CanBusApp::handleDataFrame(cMessage *msg) {
    ArbMsg *self = new ArbMsg("idle");
    DataFrame *df = check_and_cast<DataFrame *>(msg);
    self->setId(df->getID());
    int length = df->getLength();
    double nextidle;
    if (ack) {
        nextidle = (length - 11) / (double) bandwidth; //Bus guckt zum ACK-Slot nach Best�tigung
    } else {
        nextidle = length / (double) bandwidth;
    }
    //Der naechste Idle-Zustand ist eigentlich die (berechnete Zeit - 1), aber hier ist wieder die Sicherheits-Bitzeit mit verrechnet
    scheduleAt(simTime() + nextidle, self);
    ack_rcvd = false;
    numSent++;
    busPort *port = (busPort*) (getParentModule()->getSubmodule("busPort"));
    port->forward_to_all(msg);
}

void CanBusApp::registerForArbitration(int id, cModule *node, simtime_t signInTime, bool rtr){

    ids.push_back(new CanID(id, node, signInTime, rtr));

    if (idle) {
        cMessage *self = new cMessage("idle_signin");
        ack_rcvd = true;
        scheduleAt(simTime() + (1 / (double) bandwidth), self);
        idle = false;
        busytimestamp = simTime();
        char buf[64];
        sprintf(buf, "state: busy");
        bubble("state: busy");
        getDisplayString().setTagArg("tt", 0, buf);
    }
}
