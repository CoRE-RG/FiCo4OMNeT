#include "fico4omnet/bus/flexray/FRBusLogic.h"

#include "fico4omnet/scheduler/flexray/SchedulerMessage_m.h"

namespace FiCo4OMNeT {

Define_Module(FRBusLogic);

void FRBusLogic::initialize() {
    rcvdStaticFrameSignal = registerSignal("receivedStaticFrame");
    rcvdDynamicFrameSignal = registerSignal("receivedDynamicFrame");
}

void FRBusLogic::finish() {

}

void FRBusLogic::handleMessage(omnetpp::cMessage *msg) {
    //stats

    if (msg->getKind() == STATIC_EVENT) {
        emit(rcvdStaticFrameSignal, msg);
    } else if (msg->getKind() == DYNAMIC_EVENT) {
        emit(rcvdDynamicFrameSignal, msg);
    }

    //colorize
    const char* arrivalGate = msg->getArrivalGate()->getBaseName();
    char outgate[20];
    strcpy(outgate, arrivalGate);
    strncat(outgate, "$o", 2);
    send(msg, outgate);
}

void FRBusLogic::colorBusy() {
    for (int gateIndex = 0;
            gateIndex < getParentModule()->gate("gate$o", 0)->getVectorSize();
            gateIndex++) {
        getParentModule()->gate("gate$i", gateIndex)->getDisplayString().setTagArg(
                "ls", 0, "yellow");
        getParentModule()->gate("gate$i", gateIndex)->getDisplayString().setTagArg(
                "ls", 1, "3");

        getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                "ls", 0, "yellow");
        getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                "ls", 1, "3");
    }
}

void FRBusLogic::colorIdle() {
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

void FRBusLogic::colorError() {
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

}
