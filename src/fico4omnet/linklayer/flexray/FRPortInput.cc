//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "./FRPortInput.h"

#include "fico4omnet/scheduler/flexray/FRScheduler.h"
#include "fico4omnet/synchronisation/flexray/FRSync.h"

//Auto-generated messages
#include "fico4omnet/scheduler/flexray/SchedulerMessage_m.h"

namespace FiCo4OMNeT {

Define_Module(FRPortInput);

FRPortInput::FRPortInput(){
    this->bandwidth = 0;
}

void FRPortInput::initialize() {
    bandwidth = getParentModule()->getParentModule()->par("bandwidth");

    rcvdSFSignal = registerSignal("receivedCompleteSF");
    rcvdDFSignal = registerSignal("receivedCompleteDF");
}

void FRPortInput::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        if (FRFrame *frMsg = dynamic_cast<FRFrame*>(msg)) {
            if (frMsg->getKind() == STATIC_EVENT) {
                emit(rcvdSFSignal, frMsg);
            } else if (frMsg->getKind() == DYNAMIC_EVENT) {
                emit(rcvdDFSignal, frMsg);
            }
        }
        send(msg, "out");
    } else if (FRFrame *frame = dynamic_cast<FRFrame*>(msg)) {
        receivedExternMessage(frame);
    }
}

void FRPortInput::receivedExternMessage(FRFrame *frMsg) {
    FRScheduler *frScheduler =
            dynamic_cast<FRScheduler*> (getParentModule()->getParentModule()->getSubmodule(
                    "frScheduler"));
    if (frMsg->getKind() == DYNAMIC_EVENT) {
        frScheduler->dynamicFrameReceived(frMsg->getByteLength(),
                static_cast<unsigned int> (frMsg->getChannel()));
    } else {
        if (frScheduler->getSlotCounter()
                == static_cast<unsigned int> (frMsg->getFrameID())){
            if (frMsg->getSyncFrameIndicator()) {
                FRSync *frSync =
                        dynamic_cast<FRSync*> (getParentModule()->getParentModule()->getSubmodule(
                                "frSync"));
                frSync->storeDeviationValue(frMsg->getFrameID(),
                        frMsg->getCycleNumber() % 2, frMsg->getChannel(),
                        frScheduler->calculateDeviationValue(), true);
            }
        } else {
            EV << "received static frame in wrong slot!\n";
            bubble("static frame in wrong slot");
            //TODO signal for stats
        }
    }
    scheduleAt(simTime() + calculateScheduleTiming(static_cast<int> (frMsg->getBitLength())), frMsg);
}

double FRPortInput::calculateScheduleTiming(int length) {

    return static_cast<double> (length) / bandwidth;
}

}
