#include "CanBusLogic.h"

static int lala = 0;

void CanBusLogic::initialize() {
    rcvdDFSignal = registerSignal("receivedDF");
    rcvdRFSignal = registerSignal("receivedRF");
    rcvdEFSignal = registerSignal("receivedEF");
    numDataFrames = 0;
    numRemoteFrames = 0;
    numErrorFrames = 0;

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

void CanBusLogic::finish() {
    simtime_t busload = (busytime / simTime()) * 100;
    if (busload == 0.0 && !idle) {
        busload = 100.0;
    }
    recordScalar("#Simulated_Time", simTime());
    recordScalar("%Busload", busload);
    double errpercentage = (numErrorFrames
            / (double) (numDataFrames + numRemoteFrames)) * 100;
    recordScalar("%Errors", errpercentage);
}

void CanBusLogic::handleMessage(cMessage *msg) {
    std::string msgClass = msg->getClassName();
    if (msg->isSelfMessage()) { //Bus ist wieder im Idle-Zustand
        if (msgClass.compare("CanDataFrame") == 0) { //Wenn zuvor eine Nachricht gesendet wurde
            sendingCompleted();
        } else if (msgClass.compare("ErrorFrame") == 0) {
            colorIdle();
            if (scheduledDataFrame != NULL) {
                cancelEvent(scheduledDataFrame);
            }
            delete scheduledDataFrame;
            scheduledDataFrame = NULL;
            errored = false;
            eraseids.clear();
        }
        grantSendingPermission();
    } else if (msgClass.compare("CanDataFrame") == 0) { // externe Nachricht
        colorBusy();
        handleDataFrame(msg);
    } else if (msgClass.compare("ErrorFrame") == 0) {
        colorError();
        handleErrorFrame(msg);
    }

    delete msg;
}

//void CanBusLogic::checkAcknowledgementReception(ArbMsg *am) {
//    if (ack_rcvd) {
//        EV<< "Nachricht bestaetigt" << endl;
//        ArbMsg *newam = new ArbMsg("SendingComplete");
//        newam->setNode(am->getNode());
//        newam->setId(am->getId());
//        scheduleAt(simTime() + (11 / (double) bandwidth),
//                newam);
//        stateok = false;
//    } else {
//        EV << "Nachricht nicht bestaetigt" << endl;
//        AckMsg *newam = new AckMsg("SendingFailed");
//        newam->setAck(false);
//        newam->setId(am->getId());
//        BusPort *port = (BusPort*) (getParentModule()->getSubmodule("busPort"));
//        port->sendMsgToNode(newam, am->getNode());
//        ArbMsg *sp = new ArbMsg("SendingPermission");
//        sp->setSignInTime(currsit);
//        sp->setId(am->getId());
//        sp->setRemotesent(true);
//        port->sendMsgToNode(sp, am->getNode());
//    }
//    delete am;
//}

void CanBusLogic::grantSendingPermission() {
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
                sendingNode = (OutputBuffer*) id->getNode(); //der Node, der einen Data frame senden möcte wird zum senden ausgewählt
                currsit = id->getSignInTime();
                sendcount++;
            }
            eraseids.push_back(it);
        }
    }
    if (sendcount > 1) {
        EV<< "More than one node sends with the same ID.\n";
        EV<< "This behavior is not allowed. Hence the simulation is stopped.\n";
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

void CanBusLogic::sendingCompleted() {
    colorIdle();
    OutputBuffer* controller = check_and_cast<OutputBuffer*>(sendingNode);
    controller->sendingCompleted(currentSendingID);
    for (unsigned int it = 0; it != eraseids.size(); it++) {
        ids.erase(eraseids.at(it));
    }
    eraseids.clear();
    errored = false;
    if (scheduledDataFrame != NULL) {
        cancelEvent(scheduledDataFrame);
    }
    scheduledDataFrame = NULL;
}

void CanBusLogic::handleDataFrame(cMessage *msg) {
    CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
    EV<<"dataframe mit canid: " << df->getCanID() << " empfangen \n";
    int length = df->getLength();
    double nextidle;
    nextidle = length / (double) bandwidth;
    //TODO Der naechste Idle-Zustand ist eigentlich die (berechnete Zeit - 1), aber hier ist wieder die Sicherheits-Bitzeit mit verrechnet; Ist das so?
    if (scheduledDataFrame != NULL) {
        cancelEvent(scheduledDataFrame);
    }
    delete (scheduledDataFrame);
    scheduledDataFrame = df->dup();
    scheduleAt(simTime() + nextidle, scheduledDataFrame);
    if (df->getRtr()) {
        emit(rcvdRFSignal, df);
        numRemoteFrames++;
    } else {
        emit(rcvdDFSignal, df);
        numDataFrames++;
    }
    send(msg->dup(), "gate$o");
}

void CanBusLogic::handleErrorFrame(cMessage *msg) {
    if (scheduledDataFrame != NULL) {
        cancelEvent(scheduledDataFrame);
    }
    if (!errored) {
        numErrorFrames++;
        ErrorFrame *ef2 = new ErrorFrame();
        scheduleAt(simTime() + (12 / (double) bandwidth), ef2); //12 - maximale L�nge eines Error-Frames
        emit(rcvdEFSignal, ef2);
        errored = true;
        send(msg->dup(), "gate$o");
    }
}

void CanBusLogic::registerForArbitration(int id, cModule *node,
        simtime_t signInTime, bool rtr) {
    Enter_Method_Silent
    ();
    ids.push_back(new CanID(id, node, signInTime, rtr));

    if (idle) {
        cMessage *self = new cMessage("idle_signin");
        scheduleAt(simTime() + (1 / (double) bandwidth), self); //TODO was hat das mit dieser +1 auf sich?
        idle = false;
        busytimestamp = simTime();
        char buf[64];
        sprintf(buf, "state: busy");
        bubble("state: busy");
        getDisplayString().setTagArg("tt", 0, buf);
    }
}

void CanBusLogic::checkoutFromArbitration(int id) {
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

void CanBusLogic::colorBusy() {
    for (int gateIndex = 0;
            gateIndex < getParentModule()->gate("gate$o", 0)->getVectorSize();
            gateIndex++) {
        getParentModule()->gate("gate$i", gateIndex)->getDisplayString().setTagArg("ls", 0, "yellow");
        getParentModule()->gate("gate$i", gateIndex)->getDisplayString().setTagArg("ls", 1, "3");

        getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg("ls", 0, "yellow");
        getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg("ls", 1, "3");


//        getParentModule()->gate("gate$i", gateIndex)->findIncomingTransmissionChannel()->getDisplayString().setTagArg(
//                "ls", 0, "yellow");
//        getParentModule()->gate("gate$i", gateIndex)->findIncomingTransmissionChannel()->getDisplayString().setTagArg(
//                "ls", 1, "3");
//
//        getParentModule()->gate("gate$o", gateIndex)->getTransmissionChannel()->getDisplayString().setTagArg(
//                "ls", 0, "yellow");
//        getParentModule()->gate("gate$o", gateIndex)->getTransmissionChannel()->getDisplayString().setTagArg(
//                "ls", 1, "3");
    }
}

void CanBusLogic::colorIdle() {
    for (int gateIndex = 0;
            gateIndex < getParentModule()->gate("gate$o", 0)->getVectorSize();
            gateIndex++) {
        getParentModule()->gate("gate$i", gateIndex)->getDisplayString().setTagArg(
                "ls", 0, "black");
        getParentModule()->gate("gate$i", gateIndex)->getDisplayString().setTagArg(
                "ls", 1, "1");

        getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                "ls", 0, "black");
        getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                "ls", 1, "1");
    }
}

void CanBusLogic::colorError() {
    for (int gateIndex = 0;
            gateIndex < getParentModule()->gate("gate$o", 0)->getVectorSize();
            gateIndex++) {
        getParentModule()->gate("gate$i", gateIndex)->getDisplayString().setTagArg(
                "ls", 0, "red");
        getParentModule()->gate("gate$i", gateIndex)->getDisplayString().setTagArg(
                "ls", 1, "3");

        getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                "ls", 0, "red");
        getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                "ls", 1, "3");
    }
}
