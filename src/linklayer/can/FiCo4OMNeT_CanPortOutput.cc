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

#include "FiCo4OMNeT_CanPortOutput.h"

namespace FiCo4OMNeT {

Define_Module(CanPortOutput);


CanPortOutput::~CanPortOutput(){
    cancelAndDelete(scheduledErrorFrame);
}

void CanPortOutput::handleReceivedErrorFrame() {
    errorReceived = true;
    if (scheduledErrorFrame != NULL && scheduledErrorFrame->isScheduled()) {
        EV<< getParentModule()->getParentModule()->getId() << ": error frame wird gedescheduled\n";
        cancelEvent(scheduledErrorFrame);
        delete scheduledErrorFrame;
        scheduledErrorFrame = NULL;
    }
}

void CanPortOutput::initialize() {
    bandwidth = getParentModule()->getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->par("bandwidth");
    errorperc = getParentModule()->getParentModule()->par("errorperc");
    scheduledErrorFrame = new ErrorFrame();
    initializeStatisticValues();
}

void CanPortOutput::initializeStatisticValues(){
    sentDFSignal = registerSignal("sentDF");
    sentRFSignal = registerSignal("sentRF");
    sendErrorsSignal = registerSignal("sendError");
    receiveErrorsSignal = registerSignal("receiveError");
}

void CanPortOutput::handleMessage(cMessage *msg) {
    if (ErrorFrame *ef = dynamic_cast<ErrorFrame *>(msg)) {
        if (!errorReceived) {
            if (ef->getKind() < 2) { //TODO magic number
                emit(sendErrorsSignal, ef);
            } else {
                emit(receiveErrorsSignal, ef);
            }
            colorError();
            send(msg, "out");
            scheduledErrorFrame = NULL;
        } else {
            delete msg;
        }
    } else {
        CanDataFrame *df;
        colorBusy();
        errorReceived = false;
        if ((df = dynamic_cast<CanDataFrame *>(msg)) && (errorperc > 0)) {
            int senderr = intuniform(0, 99);
            if (senderr < errorperc) {
                ErrorFrame *errself = new ErrorFrame("senderror");
                int position = intuniform(0, static_cast<int> (df->getBitLength()) - MAXERRORFRAMESIZE);
                errself->setKind(intuniform(0, 1)); //0: Bit-Error, 1: Form-Error
                errself->setCanID(df->getCanID());
                if (position > 0)
                    position--;
                errself->setPos(position);
                if (scheduledErrorFrame != NULL && scheduledErrorFrame->isScheduled()) {
                    cancelEvent(scheduledErrorFrame);
                    delete(scheduledErrorFrame);
                    scheduledErrorFrame = NULL;
                }
                scheduledErrorFrame = errself;
                scheduleAt((simTime() + calculateScheduleTiming(position)),
                        scheduledErrorFrame);
            }
        }
        if (df->getRtr()) {
            emit(sentRFSignal, df);
        } else {
            emit(sentDFSignal, df);
        }
        send(df, "out");
    }
}

double CanPortOutput::calculateScheduleTiming(int length) {
    return static_cast<double> (length) / (bandwidth);
}

void CanPortOutput::sendingCompleted(){
    colorIdle();
}

void CanPortOutput::colorBusy(){
    getParentModule()->getParentModule()->getDisplayString().setTagArg("i", 1, "yellow");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 0, "yellow");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 1, "3");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 0, "yellow");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 1, "3");
}

void CanPortOutput::colorIdle(){
    getParentModule()->getParentModule()->getDisplayString().setTagArg("i", 1, "");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 0, "black");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 1, "1");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 0, "black");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 1, "1");
}

void CanPortOutput::colorError(){
    getParentModule()->getParentModule()->getDisplayString().setTagArg("i", 1, "red");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 0, "red");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 1, "3");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 0, "red");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 1, "3");
}

}
