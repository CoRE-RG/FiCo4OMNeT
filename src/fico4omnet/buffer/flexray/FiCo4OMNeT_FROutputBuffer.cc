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

#include "FiCo4OMNeT_FROutputBuffer.h"

namespace FiCo4OMNeT {

Define_Module(FROutputBuffer);

void FROutputBuffer::putFrame(cMessage* msg) {
    FRFrame *frame = dynamic_cast<FRFrame*>(msg);
    if (getFrame(frame->getFrameID()) != NULL) {
        deleteFrame(frame->getFrameID());
    }
    frames.push_back(frame);
}

void FROutputBuffer::sendingCompleted(int id) {
    Enter_Method_Silent
    ();
    deleteFrame(id);
}

void FROutputBuffer::handleMessage(cMessage *msg) {
    take(msg);
    if (SchedulerActionTimeEvent *event =
            dynamic_cast<SchedulerActionTimeEvent *>(msg)) {
        deliverFrame(static_cast<int> (event->getFrameID()));
        delete msg;
    } else {
        FRBuffer::handleMessage(msg);
        if (FRFrame * frame = dynamic_cast<FRFrame*>(msg)) {
            FRScheduler *frScheduler =
                    dynamic_cast<FRScheduler*> (getParentModule()->getSubmodule(
                            "frScheduler"));
            if (frame->getKind() == STATIC_EVENT) {
                event = new SchedulerActionTimeEvent("Static Event",
                        STATIC_EVENT);
            } else if (frame->getKind() == DYNAMIC_EVENT) {
                event = new SchedulerActionTimeEvent("Dynamic Event",
                        DYNAMIC_EVENT);
            } else {
                throw cRuntimeError("The FROutputBuffer %s received a wrong message.", this->getFullPath().c_str());
            }
            event->setFrameID(static_cast<unsigned int> (frame->getFrameID()));
            event->setChannel(static_cast<unsigned int> (frame->getChannel()));
            event->setCycleNr(static_cast<unsigned int> (frame->getCycleNumber()));
            event->setDestinationGate(this->gate("schedulerIn"));
            frScheduler->registerEvent(event);
        }
    }
}

}
