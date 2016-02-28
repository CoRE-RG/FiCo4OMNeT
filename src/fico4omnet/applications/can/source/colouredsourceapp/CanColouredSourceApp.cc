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

#include "./CanColouredSourceApp.h"

#include "fico4omnet/scheduler/can/CanClock.h"

namespace FiCo4OMNeT {

Define_Module(CanColouredSourceApp)
;

CanColouredSourceApp::CanColouredSourceApp() : CanTrafficSourceAppBase::CanTrafficSourceAppBase()
{
}

CanColouredSourceApp::~CanColouredSourceApp()
{
}

void CanColouredSourceApp::initialize(int stage) {
    frameDisplayString = par("displayString");
    CanTrafficSourceAppBase::initialize(stage);
}

void CanColouredSourceApp::handleMessage(cMessage *msg) {
    CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
    frameTransmission(df);
}

void CanColouredSourceApp::frameTransmission(CanDataFrame *df) {
    CanDataFrame *outgoingFrame = NULL;

    if (df->getRtr()) {
        emit(sentRFSignal, df);
    } else {
        emit(sentDFSignal, df);
    }

    if (df->isSelfMessage()) {
        outgoingFrame = df->dup();
        CanClock* canClock =
                dynamic_cast<CanClock*>(getParentModule()->getSubmodule("canClock"));
        currentDrift = canClock->getCurrentDrift();
        scheduleAt(
                simTime() + (df->getPeriod())
                        + SimTime(par("periodInaccurracy").doubleValue() + currentDrift), df);
    } else if (df->arrivedOn("remoteIn")) {
        for (std::list<CanDataFrame*>::iterator it =
                outgoingDataFrames.begin(); it != outgoingDataFrames.end();
                ++it) {
            CanDataFrame *tmp = *it;
            if (tmp->getCanID() == df->getCanID()) {
                outgoingFrame = tmp->dup();
                break;
            }
        }
        delete df;
    } else {
        throw cRuntimeError("CanColouredSourceApp received an invalid message.");
    }

    outgoingFrame->setTimestamp(simTime());
    cPacket* payload_packet = outgoingFrame->decapsulate();
    payload_packet->setTimestamp(simTime());
    outgoingFrame->encapsulate(payload_packet);
    outgoingFrame->setDisplayString(frameDisplayString);
    send(outgoingFrame, "out");
}

}
