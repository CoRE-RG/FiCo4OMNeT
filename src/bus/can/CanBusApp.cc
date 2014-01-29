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
    scheduledDataFrame = new CanDataFrame();

    payload = getParentModule()->par("payload");
    errors = getParentModule()->par("errors");
    ack = getParentModule()->par("ack");
    bandwidth = getParentModule()->par("bandwidth");
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

void CanBusApp::handleMessage(cMessage *msg) {
    take(msg);
    std::string msgClass = msg->getClassName();
    if (msg->isSelfMessage()) { //Bus ist wieder im Idle-Zustand
//        BusPort *port = (BusPort*) (getParentModule()->getSubmodule("busPort"));
//        string name = msg->getName();
//
//        if (name.compare("idle") == 0) { //Wenn zuvor eine Nachricht gesendet wurde
        if (msgClass.compare("CanDataFrame") == 0) { //Wenn zuvor eine Nachricht gesendet wurde

//            if (errorcount == 0 || !errors) {
//                ArbMsg *am = check_and_cast<ArbMsg *>(msg);
//
//                if (ack) {
//                    checkAcknowledgementReception(am);
//                } else {
//                    OutputBuffer* controller = check_and_cast<OutputBuffer*>(
//                            sendingNode);
//                    controller->sendingCompleted(currentSendingID);
//                    stateok = true;
//                }
            sendingCompleted();

//            } else {
//                errorcount--;
//            }
//        } else if (name.compare("SendingComplete") == 0) {
//            ArbMsg *am = check_and_cast<ArbMsg *>(msg);
//            AckMsg *newam = new AckMsg("SendingComplete");
//            newam->setId(am->getId());
//            stateok = true;
//            port->sendMsgToNode(newam, am->getNode());
//
//        } else if (name.compare("ErrResend") == 0) {
//            ArbMsg *ef = check_and_cast<ArbMsg *>(msg);
//            ArbMsg *sp = new ArbMsg("ErrResendCompl");
//            sp->setRemotesent(true);
//            sp->setId(ef->getId());
//            sp->setNode(ef->getNode());
//            scheduleAt(simTime() + (errpos / (double) bandwidth), sp);
//            errored = false;
//            stateok = false;
//            errpos = INT_MAX;
//
//        } else if (name.compare("ErrResendCompl") == 0) {
//            ArbMsg *ef = check_and_cast<ArbMsg *>(msg);
//            ArbMsg *sp = new ArbMsg("SendingPermission");
//            sp->setSignInTime(currsit);
//            sp->setRemotesent(true);
//            sp->setId(ef->getId());
//            vector<list<CanID*>::iterator> eraseids;
//            for (list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) {
//                CanID *id = *it;
//                if (id->getId() == ef->getId()) {
//                    eraseids.push_back(it);
//                }
//            }
//            for (unsigned int it = 0; it != eraseids.size(); it++) {
//                ids.erase(eraseids.at(it));
//            }
//            port->sendMsgToNode(sp, ef->getNode());
        } else if (msgClass.compare("ErrorFrame") == 0) {
            scheduledDataFrame = new CanDataFrame();
            errored = false;
            eraseids.clear();
        }
        grantSendingPermission();
        ack_rcvd = true;

    } else if (msgClass.compare("CanDataFrame") == 0) { // externe Nachricht
        handleDataFrame(msg);
    } else if (msgClass.compare("ErrorFrame") == 0) {
        handleErrorFrame(msg);
    }
    delete msg;
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
        BusPort *port = (BusPort*) (getParentModule()->getSubmodule("busPort"));
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
    sendingNode = NULL;

    for (list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) { //finden der höchsten Priorität aller angemeldeten Nachrichten
        CanID *id = *it;
        if (id->getId() < currentSendingID) {
            currentSendingID = id->getId();
            sendingNode = (OutputBuffer*) id->getNode();
            currsit = id->getSignInTime();
        }
    }

    int sendcount = 0;
    for (list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) { //finden, ob remote frame für diese ID auch gesendet werden soll
        CanID *id = *it;
        if (id->getId() == currentSendingID) {
            if (id->getRtr() == false) { //Data-Frame
//                if (sendingNode != id->getNode()) { //bei dem ursprünglich gefundenen node handelt es sich um einen remote frame
                sendingNode = (OutputBuffer*) id->getNode(); //der Node, der einen Data frame senden möcte wird zum senden ausgewählt
                currsit = id->getSignInTime();
                sendcount++;
//                }
            }
            eraseids.push_back(it);
        }
    }
    if (sendcount > 1) {
        EV<< "Mehr als ein Knoten will mit der selben ID senden\n";
        EV<< "Da dies zu fortlaufenden Bit-Fehlern durch die versendenden Knoten fuehrt, wird die Simulation angehalten\n";
        endSimulation();
    }
    if (sendingNode != NULL) {
        OutputBuffer* controller = check_and_cast<OutputBuffer *>(sendingNode);
        controller->receiveSendingPermission(currentSendingID);
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

void CanBusApp::sendingCompleted() {
    OutputBuffer* controller = check_and_cast<OutputBuffer*>(sendingNode);
    controller->sendingCompleted(currentSendingID);
    for (unsigned int it = 0; it != eraseids.size(); it++) {
        ids.erase(eraseids.at(it));
    }
    eraseids.clear();
    errored = false;
    scheduledDataFrame = new CanDataFrame();
}

void CanBusApp::handleDataFrame(cMessage *msg) {
    CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
    int length = df->getLength();
    double nextidle;
    nextidle = length / (double) bandwidth;
    //TODO Der naechste Idle-Zustand ist eigentlich die (berechnete Zeit - 1), aber hier ist wieder die Sicherheits-Bitzeit mit verrechnet; Ist das so?
    scheduledDataFrame = df->dup();
    scheduleAt(simTime() + nextidle, scheduledDataFrame);
    ack_rcvd = false;
    numSent++;
    BusPort *port = (BusPort*) (getParentModule()->getSubmodule("busPort"));
    port->forward_to_all(msg->dup());
}

void CanBusApp::handleErrorFrame(cMessage *msg) {
    if (scheduledDataFrame->isScheduled()) {
        cancelEvent(scheduledDataFrame);
    }
    if (!errored) {
//        delete scheduledDataFrame;
//        scheduledDataFrame = new CanDataFrame();
        ErrorFrame *ef = check_and_cast<ErrorFrame *>(msg);
        scheduleAt(simTime() + (12 / (double) bandwidth), ef->dup()); //12 - maximale L�nge eines Error-Frames
        errorcount++;
        numErr++;
        errored = true;
        BusPort *port = (BusPort*) (getParentModule()->getSubmodule("busPort"));
        port->forward_to_all(msg->dup());
    }
}

void CanBusApp::registerForArbitration(int id, cModule *node,
        simtime_t signInTime, bool rtr) {
    Enter_Method_Silent
    ();
    ids.push_back(new CanID(id, node, signInTime, rtr));

    if (idle) {
        cMessage *self = new cMessage("idle_signin");
        ack_rcvd = true;
        scheduleAt(simTime() + (1 / (double) bandwidth), self); //TODO was hat das mit dieser +1 auf sich?
        idle = false;
        busytimestamp = simTime();
        char buf[64];
        sprintf(buf, "state: busy");
        bubble("state: busy");
        getDisplayString().setTagArg("tt", 0, buf);
    }
}

void CanBusApp::checkoutFromArbitration(int id) {
    Enter_Method_Silent
    ();
    for (list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) {
        CanID* tmp = *it;
        if (tmp->getId() == id) {
            ids.remove(tmp);
        }
        break;
    }
}
