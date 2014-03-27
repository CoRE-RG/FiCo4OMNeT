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

#include "FRTrafficSinkAppBase.h"

void FRTrafficSinkAppBase::initialize() {
    idle = true;
    currentFrameID = 0;
    bufferMessageCounter = 0;
}

void FRTrafficSinkAppBase::handleMessage(cMessage *msg) {
//    std::string msgClass = msg->getClassName();
    if (msg->arrivedOn("controllerIn")) {
        bufferMessageCounter++;
        if (idle) {
            requestFrame();
        }
//    } else if (msgClass.compare("CanDataFrame") == 0) {
    } else if (FRFrame * frame = dynamic_cast<FRFrame *> (msg)) {
//        FRFrame *frame = check_and_cast<FRFrame *>(msg);
        int i = frame->getId();
        currentFrameID = i;
        bufferMessageCounter--;
        startWorkOnFrame(0); //TODO working time
    } else if (msg->isSelfMessage()) {
        FRInputBuffer *buffer = (FRInputBuffer*) (getParentModule()->getSubmodule(
                "bufferIn"));
        buffer->deleteFrame(currentFrameID);
        if (bufferMessageCounter > 0) {
            requestFrame();
        } else {
            idle = true;
        }
    }
    delete msg;
}

void FRTrafficSinkAppBase::requestFrame() {
    FRInputBuffer *buffer = (FRInputBuffer*) (getParentModule()->getSubmodule(
            "bufferIn"));
    buffer->deliverNextFrame();
    idle = false;
}

void FRTrafficSinkAppBase::startWorkOnFrame(float workTime) {
    cMessage *msg = new cMessage("workFinished");
    scheduleAt(simTime() + workTime, msg);
}
