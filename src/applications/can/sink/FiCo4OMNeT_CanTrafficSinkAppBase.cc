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

#include "FiCo4OMNeT_CanTrafficSinkAppBase.h"

namespace FiCo4OMNeT {

Define_Module(CanTrafficSinkAppBase)
;

void CanTrafficSinkAppBase::initialize() {
    idle = true;
    currentFrameID = 0;
    bufferMessageCounter = 0;

    rcvdDFSignal = registerSignal("receivedCompleteDF");
    rcvdRFSignal = registerSignal("receivedCompleteRF");
    rcvdDFSignalFromNode = registerSignal("receivedCompleteDFFromNode");
    rcvdRFSignalFromNode = registerSignal("receivedCompleteRFFromNode");
    rcvdDFSignalFromGW = registerSignal("receivedCompleteDFFromGW");
    rcvdRFSignalFromGW = registerSignal("receivedCompleteRFFromGW");
//    registerIncomingDataFramesAtPort();
}

//void CanTrafficSinkAppBase::registerIncomingDataFramesAtPort() {
//    CanPortInput* port = (CanPortInput*) getParentModule()->getSubmodule(
//            "canNodePort")->getSubmodule("canPortInput");
//    cStringTokenizer idIncomingFramesTokenizer(par("idIncomingFrames"), ",");
//
//    while (idIncomingFramesTokenizer.hasMoreTokens()){
//        std::stringstream strValue;
//        int intValue;
//        strValue << idIncomingFramesTokenizer.nextToken();
//        strValue >> intValue;
//        port->registerIncomingDataFrame(intValue);
//    }
//}

void CanTrafficSinkAppBase::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("controllerIn")) {
        bufferMessageCounter++;
        if (idle) {
            requestFrame();
        }
    } else if (CanDataFrame *frame = dynamic_cast<CanDataFrame *>(msg)) {
        int i = frame->getCanID();
        currentFrameID = i;
        bufferMessageCounter--;
        if (frame->getRtr()) {
            emit(rcvdRFSignal, frame);
            if (frame->getMessageSource() == SOURCE_NODE) {
                emit(rcvdRFSignalFromNode, frame);
            } else if (frame->getMessageSource() == SOURCE_GW) {
                emit(rcvdRFSignalFromGW, frame);
            }
        } else {
            emit(rcvdDFSignal, frame);
            if (frame->getMessageSource() == SOURCE_NODE) {
                emit(rcvdDFSignalFromNode, frame);
            } else if (frame->getMessageSource() == SOURCE_GW) {
                emit(rcvdDFSignalFromGW, frame);
            }
        }
        startWorkOnFrame(0);
    } else if (msg->isSelfMessage()) {
        CanInputBuffer *buffer = dynamic_cast<CanInputBuffer*> (getParentModule()->getSubmodule("bufferIn"));
        buffer->deleteFrame(currentFrameID);
        if (bufferMessageCounter > 0) {
            requestFrame();
        } else {
            idle = true;
        }
    }
    delete msg;
}

void CanTrafficSinkAppBase::requestFrame() {
    CanInputBuffer *buffer = dynamic_cast<CanInputBuffer*> (getParentModule()->getSubmodule("bufferIn"));
    buffer->deliverNextFrame();
    idle = false;
}

void CanTrafficSinkAppBase::startWorkOnFrame(float workTime) {

    (void)workTime;

//    cMessage *msg = new cMessage("workFinished");
//    scheduleAt(simTime() + workTime, msg);
}

}
