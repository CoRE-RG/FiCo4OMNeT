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

#include "FiCo4OMNeT_CanPortInput.h"

namespace FiCo4OMNeT {

Define_Module(CanPortInput);

void CanPortInput::initialize() {

//    cStringTokenizer idIncomingFramesTokenizer(
//            getParentModule()->getParentModule()->par("idIncomingFrames"), ",");
//    incomingDataFrameIDs = idIncomingFramesTokenizer.asIntVector();
//    cStringTokenizer idIncomingRemoteFramesTokenizer(
//            getParentModule()->getParentModule()->par("idDataFrames"), ",");
//    cStringTokenizer idOutgoingRemoteFramesTokenizer(
//            getParentModule()->getParentModule()->par("idRemoteFrames"), ",");
//    outgoingRemoteFrameIDs = idIncomingRemoteFramesTokenizer.asIntVector();

    bandwidth = getParentModule()->getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->par("bandwidth");
    errorperc = getParentModule()->getParentModule()->par("errorperc");

    scheduledDataFrame = NULL;
    scheduledErrorFrame = NULL;

    rcvdDFSignal = registerSignal("receivedCompleteDF");
    rcvdRFSignal = registerSignal("receivedCompleteRF");

    WATCH_VECTOR(incomingDataFrameIDs);
}

void CanPortInput::handleMessage(cMessage *msg) {
    std::string msgClass = msg->getClassName();
    if (msg->isSelfMessage()) {
        if (ErrorFrame *ef = dynamic_cast<ErrorFrame *>(msg)) {
            forwardOwnErrorFrame(ef);
            if (scheduledErrorFrame != NULL) {
                cancelEvent(scheduledErrorFrame);
            }
            delete (scheduledErrorFrame);
            scheduledErrorFrame = NULL;
        } else if (CanDataFrame *df = dynamic_cast<CanDataFrame *>(msg)) {
            forwardDataFrame(df);
            if (scheduledDataFrame != NULL) {
                cancelEvent(scheduledDataFrame);
            }
            delete (scheduledDataFrame);
            scheduledDataFrame = NULL;
        }
    } else if (CanDataFrame *df = dynamic_cast<CanDataFrame *>(msg)) {
        if (checkExistence(df)) {
            int rcverr = intuniform(0, 99);
            if (rcverr < errorperc) {
                generateReceiveError(df);
            } else {
                receiveMessage(df);
            }
        } else {
            if (scheduledDataFrame != NULL) {
                cancelEvent(scheduledDataFrame);
            }
            dropAndDelete(scheduledDataFrame);
            scheduledDataFrame = NULL;
        }
        delete msg;
    } else if (ErrorFrame *ef = dynamic_cast<ErrorFrame *>(msg)) {
        handleExternErrorFrame(ef);
        delete msg;
    }
}

void CanPortInput::receiveMessage(CanDataFrame *df) {
    int frameLength = df->getLength();
    if (scheduledDataFrame != NULL) {
        cancelEvent(scheduledDataFrame);
    }
    delete (scheduledDataFrame);
    scheduledDataFrame = df->dup();
    scheduleAt((simTime() + calculateScheduleTiming(frameLength)),
            scheduledDataFrame);
}

void CanPortInput::generateReceiveError(CanDataFrame *df) {
    ErrorFrame *errorMsg = new ErrorFrame("receiveError");
    int pos = intuniform(0, df->getLength() - MAXERRORFRAMESIZE);
    errorMsg->setKind(intuniform(2, 3)); //2: CRC-error, 3: Bit-Stuffing-error
    errorMsg->setCanID(df->getCanID());
    if (pos > 0)
        pos--;
    errorMsg->setPos(pos);
    if (scheduledErrorFrame != NULL) {
        cancelEvent(scheduledErrorFrame);
    }
    delete (scheduledErrorFrame);
    scheduledErrorFrame = errorMsg;
    scheduleAt((simTime() + calculateScheduleTiming(pos)), scheduledErrorFrame);
}

bool CanPortInput::checkExistence(CanDataFrame *df) {
    if (df->getRtr()) {
        return checkOutgoingDataFrames(df->getCanID());
    } else {
        return checkIncomingDataFrames(df->getCanID());
    }
}

bool CanPortInput::checkOutgoingDataFrames(int id) {
    std::map<int,cGate*>::iterator it;
    it = outgoingDataFrameIDs.find(id);
    if (it != outgoingDataFrameIDs.end()) {
        return true;
    }
    return false;
}

bool CanPortInput::checkOutgoingRemoteFrames(int id) {
    for (std::vector<int>::iterator it = outgoingRemoteFrameIDs.begin();
            it != outgoingRemoteFrameIDs.end(); ++it) {
        if (*it == id) {
            return true;
        }
    }
    return false;
}

bool CanPortInput::checkIncomingDataFrames(int id) {
    for (std::vector<int>::iterator it = incomingDataFrameIDs.begin();
            it != incomingDataFrameIDs.end(); ++it) {
        if (*it == id) {
            return true;
        }
    }
    return false;
}

double CanPortInput::calculateScheduleTiming(int length) {
    return ((double) length) / (bandwidth);
}

void CanPortInput::forwardDataFrame(CanDataFrame *df) {
    if (df->getRtr()) {
        std::map<int,cGate*>::iterator it;
        it = outgoingDataFrameIDs.find(df->getCanID());
        if (it!=outgoingDataFrameIDs.end()) {
            emit(rcvdRFSignal, df);
            sendDirect(df->dup(), it->second);
        }
    } else {
        for (std::vector<int>::iterator it = incomingDataFrameIDs.begin();
                it != incomingDataFrameIDs.end(); ++it) {
            if (*it == df->getCanID()) {
                emit(rcvdDFSignal, df);
                send(df->dup(), "out");
                break;
            }
        }
    }
}

void CanPortInput::forwardOwnErrorFrame(ErrorFrame *ef) {
    cModule* portOutput = getParentModule()->getSubmodule("canPortOutput");
    sendDirect(ef->dup(), portOutput, "directIn");
}

void CanPortInput::handleExternErrorFrame(ErrorFrame *ef) {
    CanPortOutput* portOutput =
            (CanPortOutput*) getParentModule()->getSubmodule("canPortOutput");
    portOutput->sendingCompleted();
    if ((checkOutgoingDataFrames(ef->getCanID())
            || checkOutgoingRemoteFrames(ef->getCanID()))) {
        if (ef->getKind() > 2) {
            portOutput->handleReceivedErrorFrame();
        }
    }
    if (scheduledDataFrame != NULL && scheduledDataFrame->isScheduled()
            && (ef->getCanID() == scheduledDataFrame->getCanID())) {
        cancelEvent(scheduledDataFrame);
    }
    if (scheduledErrorFrame != NULL && scheduledErrorFrame->isScheduled()
            && ef->getCanID() == scheduledErrorFrame->getCanID()) {
        cancelEvent(scheduledErrorFrame);
    }
}

void CanPortInput::registerOutgoingDataFrame(int canID, cGate* gate){
    outgoingDataFrameIDs.insert(std::pair<int,cGate*>(canID,gate));
}

void CanPortInput::registerOutgoingRemoteFrame(int canID){
    std::vector<int>::iterator it;
    it = outgoingRemoteFrameIDs.begin();
    it = outgoingRemoteFrameIDs.insert(it, canID);
}

void CanPortInput::registerIncomingDataFrame(int canID){
    std::vector<int>::iterator it;
    it = incomingDataFrameIDs.begin();
    it = incomingDataFrameIDs.insert(it, canID);
}

}
