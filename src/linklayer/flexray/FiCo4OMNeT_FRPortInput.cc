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

#include "FiCo4OMNeT_FRPortInput.h"

namespace FiCo4OMNeT {

Define_Module(FRPortInput);

void FRPortInput::initialize() {
    bandwidth = getParentModule()->getParentModule()->par("bandwidth");
}

void FRPortInput::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        send(msg, "out");
    } else if (FRFrame *frame = dynamic_cast<FRFrame*>(msg)){
        receivedExternMessage(frame);
    }
}

void FRPortInput::receivedExternMessage(FRFrame *msg) {
    FRFrame *frMsg = dynamic_cast<FRFrame*>(msg);
    FRScheduler *frScheduler =
            (FRScheduler*) (getParentModule()->getParentModule()->getSubmodule(
                    "frScheduler"));
    if (frMsg->getKind() == DYNAMIC_EVENT) {
        frScheduler->dynamicFrameReceived(frMsg->getSize(),
                frMsg->getChannel());
    } else {
        if (frScheduler->getSlotCounter()
                == (unsigned int) frMsg->getFrameID()) {
            if (frMsg->getSyncFrameIndicator()) {
                FRSync *frSync =
                        (FRSync*) (getParentModule()->getParentModule()->getSubmodule(
                                "frSync")); //TODO in init. geht das?
                frSync->storeDeviationValue(frMsg->getFrameID(),
                        frMsg->getCycleNumber() % 2, frMsg->getChannel(),
                        frScheduler->calculateDeviationValue(), true);
            }
        } else {
            EV<< "received static frame in wrong slot!\n";
            bubble("static frame in wrong slot");
        }
    }
    scheduleAt(simTime() + calculateScheduleTiming(frMsg->getSize()), frMsg);
}

double FRPortInput::calculateScheduleTiming(int length) {
    return ((double) length) / bandwidth;
}

}
