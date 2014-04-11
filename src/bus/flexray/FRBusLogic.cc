#include "FRBusLogic.h"

void FRBusLogic::initialize() {

}

void FRBusLogic::finish() {

}

void FRBusLogic::handleMessage(cMessage *msg) {
    //stats
    //colorize
    const char* gate = msg->getArrivalGate()->getBaseName();
    char outgate [20];
    strcpy(outgate,gate);
    strncat(outgate,"$o",2);
    send(msg,outgate);
}


void FRBusLogic::colorBusy() {
    for (int gateIndex = 0;
            gateIndex < getParentModule()->gate("gate$o", 0)->getVectorSize();
            gateIndex++) {
        getParentModule()->gate("gate$i", gateIndex)->getDisplayString().setTagArg("ls", 0, "yellow");
        getParentModule()->gate("gate$i", gateIndex)->getDisplayString().setTagArg("ls", 1, "3");

        getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg("ls", 0, "yellow");
        getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg("ls", 1, "3");
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
