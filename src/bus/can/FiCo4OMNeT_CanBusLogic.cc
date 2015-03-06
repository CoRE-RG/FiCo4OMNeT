//Copyright (c) 2014, CoRE Research Group, Hamburg University of Applied Sciences
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification,
//are permitted provided that the following conditions are met:
//
//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
//3. Neither the name of the copyright holder nor the names of its contributors
//   may be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
//ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "FiCo4OMNeT_CanBusLogic.h"

namespace FiCo4OMNeT {

Define_Module(CanBusLogic);

CanBusLogic::CanBusLogic() {
    numDataFrames = 0;
    numRemoteFrames = 0;
    numErrorFrames = 0;

    busytime = 0.0;
    busytimestamp = 0.0;
    errpos = INT_MAX;
    errored = false;
    idle = true;

    scheduledDataFrame = new CanDataFrame();

    numFramesSent = 0;
    numBitsSent = 0;
}
CanBusLogic::~CanBusLogic() {
    if(scheduledDataFrame){
        cancelAndDelete(scheduledDataFrame);
    }
}

void CanBusLogic::initialize() {
    rcvdSignal = registerSignal("received");
    rcvdDFSignal = registerSignal("receivedDF");
    rcvdRFSignal = registerSignal("receivedRF");
    rcvdEFSignal = registerSignal("receivedEF");
    stateSignal = registerSignal("state");

    char buf[64];
    sprintf(buf, "state: idle");
    bubble("state: idle");
    getDisplayString().setTagArg("tt", 0, buf);


    bandwidth = getParentModule()->par("bandwidth");
}

void CanBusLogic::finish() {
    EV << "busytime: " << busytime << endl;
    EV << "simtime: " << simTime() << endl;
    simtime_t busload = (busytime / simTime()) * 100;
    if (busload == 0.0 && !idle) {
        busload = 100.0;
    }
    recordScalar("#Simulated_Time", simTime());
    recordScalar("%Busload", busload);
    double errpercentage = (static_cast<double> (numErrorFrames)
            / static_cast<double> (numDataFrames + numRemoteFrames)) * 100;
    recordScalar("%Errors", errpercentage);


    simtime_t t = simTime();
    if(t > 0){
        recordScalar("frames/sec", static_cast<double> (numFramesSent) / t);
        recordScalar("bits/sec", static_cast<double> (numBitsSent) / t);
    }
}

int CanBusLogic::getSendingNodeID() {
    if (sendingNode != NULL) {
        return sendingNode->getId();
    }
    return -1;
}

void CanBusLogic::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (dynamic_cast<CanDataFrame *>(msg)) {
            sendingCompleted();
        } else if (dynamic_cast<ErrorFrame *>(msg)) {
            colorIdle();
            emit(stateSignal, IDLE);
            if (scheduledDataFrame != NULL) {
                cancelEvent(scheduledDataFrame);
            }
            delete scheduledDataFrame;
            scheduledDataFrame = NULL;
            errored = false;
            eraseids.clear();
        }
        grantSendingPermission();
    } else if (dynamic_cast<CanDataFrame *>(msg)) {
        colorBusy();
        emit(stateSignal, TRANSMITTING);
        handleDataFrame(msg);
    } else if (dynamic_cast<ErrorFrame *>(msg)) {
        colorError();
        handleErrorFrame(msg);
    }

    delete msg;
}

void CanBusLogic::grantSendingPermission() {
    currentSendingID = INT_MAX;
    sendingNode = NULL;

    for (std::list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) {
        CanID *id = *it;
        if (id->getId() < currentSendingID) {
            currentSendingID = id->getId();
            sendingNode = dynamic_cast<CanOutputBuffer*> (id->getNode());
            currsit = id->getSignInTime();
        }
    }

    int sendcount = 0;
    bool nodeFound = false;
    for (std::list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) {
        CanID *id = *it;
        if (id->getId() == currentSendingID) {
            if (id->getRtr() == false) { //Data-Frame
                sendcount++;
                if (!nodeFound) {
                    nodeFound = true;
                    sendingNode = dynamic_cast<CanOutputBuffer*> (id->getNode());
                    currsit = id->getSignInTime();
                    eraseids.push_back(it);
                }
            } else {
                eraseids.push_back(it);
            }
        }
    }

    if (sendcount > 1) {
        cComponent::bubble("More than one node sends with the same ID.");
//        std::ostringstream oss;
//        oss << "More than one node sends with the same ID " << currentSendingID
//                << ". This behavior is not allowed.";
//        throw cRuntimeError(oss.str().c_str());
    }
    if (sendingNode != NULL) {
        CanOutputBuffer* controller = check_and_cast<CanOutputBuffer *>(
                sendingNode);
        controller->receiveSendingPermission(currentSendingID);
    } else {
        EV << "no pending message" << endl;
        simtime_t timetaken = simTime() - busytimestamp;
        busytime += timetaken;
        EV << "Busytime: " << busytime << "\n";
        idle = true;
        getDisplayString().setTagArg("tt", 0, "state: idle");
        bubble("state: idle");
    }
}

void CanBusLogic::sendingCompleted() {
    colorIdle();
    emit(stateSignal, IDLE);
    CanOutputBuffer* controller = check_and_cast<CanOutputBuffer*>(sendingNode);
    controller->sendingCompleted();
    for (unsigned int it = 0; it != eraseids.size(); it++) {
        ids.erase(eraseids.at(it));
        delete *(eraseids.at(it));
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
    int64_t length = df->getBitLength();
    double nextidle;
    nextidle = static_cast<double> (length) / bandwidth;
    if (scheduledDataFrame != NULL) {
        cancelEvent(scheduledDataFrame);
    }
    delete (scheduledDataFrame);
    scheduledDataFrame = df->dup();
    scheduleAt(simTime() + nextidle, scheduledDataFrame);
    emit(rcvdSignal, df);
    if (df->getRtr()) {
        emit(rcvdRFSignal, df);
        numRemoteFrames++;
    } else {
        emit(rcvdDFSignal, df);
        numDataFrames++;
    }
    send(msg->dup(), "gate$o");
    numFramesSent++;
    numBitsSent += static_cast<unsigned long> (df->getBitLength());
}

void CanBusLogic::handleErrorFrame(cMessage *msg) {
    if (scheduledDataFrame != NULL) {
        cancelEvent(scheduledDataFrame);
    }
    if (!errored) {
        numErrorFrames++;
        ErrorFrame *ef2 = new ErrorFrame();
        scheduleAt(simTime() + (MAXERRORFRAMESIZE / (bandwidth)), ef2);
        emit(rcvdEFSignal, ef2);
        errored = true;
        send(msg->dup(), "gate$o");

        //TODO Errorframes statistic?!?!
        //numFramesSent++;
        //numBitsSent+=df->getLength();
    }
}

void CanBusLogic::registerForArbitration(unsigned int id, cModule *node,
        simtime_t signInTime, bool rtr) {
    Enter_Method_Silent
    ();
    ids.push_back(new CanID(id, node, signInTime, rtr));
    if (idle) {
        cMessage *self = new cMessage("idle_signin");
        scheduleAt(simTime() + (1 / (bandwidth)), self);
        idle = false;
        busytimestamp = simTime();
        bubble("state: busy");
        getDisplayString().setTagArg("tt", 0, "state: busy");
        emit(stateSignal, TRANSMITTING);
    }
}

void CanBusLogic::checkoutFromArbitration(unsigned int canID) {
    Enter_Method_Silent
    ();
    for (std::list<CanID*>::iterator it = ids.begin(); it != ids.end(); ++it) {
        CanID* tmp = *it;
        if (tmp->getId() == canID) {
            ids.remove(tmp);
            delete tmp;
            break;
        }
    }
}

void CanBusLogic::colorBusy() {
    if (ev.isGUI()) {
        for (int gateIndex = 0;
                gateIndex
                        < getParentModule()->gate("gate$o", 0)->getVectorSize();
                gateIndex++) {
            getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                    "ls", 0, "yellow");
            getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                    "ls", 1, "3");

            //TODO: This is necessary due to visualization issues with OMNeT++
            getParentModule()->gate("gate$i", gateIndex)->getPreviousGate()->getDisplayString().setTagArg(
                    "ls", 0, "yellow");
            getParentModule()->gate("gate$i", gateIndex)->getPreviousGate()->getDisplayString().setTagArg(
                    "ls", 1, "3");
        }
    }
}

void CanBusLogic::colorIdle() {
    if (ev.isGUI()) {
        for (int gateIndex = 0;
                gateIndex
                        < getParentModule()->gate("gate$o", 0)->getVectorSize();
                gateIndex++) {
            getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                    "ls", 0, "black");
            getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                    "ls", 1, "1");

            //TODO: This is necessary due to visualization issues with OMNeT++
            getParentModule()->gate("gate$i", gateIndex)->getPreviousGate()->getDisplayString().setTagArg(
                    "ls", 0, "black");
            getParentModule()->gate("gate$i", gateIndex)->getPreviousGate()->getDisplayString().setTagArg(
                    "ls", 1, "1");
        }
    }
}

void CanBusLogic::colorError() {
    if (ev.isGUI()) {
        for (int gateIndex = 0;
                gateIndex
                        < getParentModule()->gate("gate$o", 0)->getVectorSize();
                gateIndex++) {
            getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                    "ls", 0, "red");
            getParentModule()->gate("gate$o", gateIndex)->getDisplayString().setTagArg(
                    "ls", 1, "3");

            //TODO: This is necessary due to visualization issues with OMNeT++
            getParentModule()->gate("gate$i", gateIndex)->getPreviousGate()->getDisplayString().setTagArg(
                    "ls", 0, "red");
            getParentModule()->gate("gate$i", gateIndex)->getPreviousGate()->getDisplayString().setTagArg(
                    "ls", 1, "3");
        }
    }
}

}
//TEST FR!!!!!!!!!!!!
