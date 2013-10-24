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

#include "CanTrafficSinkApp.h"

CanTrafficSinkApp::CanTrafficSinkApp() {
    // TODO Auto-generated constructor stub

}

CanTrafficSinkApp::~CanTrafficSinkApp() {
    // TODO Auto-generated destructor stub
}

void CanTrafficSinkApp::initialize() {
    //TODO init statistics
    //TODO bei Buffer registrieren?
    idle = true;
    currentFrameID = 0;
}

void CanTrafficSinkApp::handleMessage(cMessage *msg) {
    //TODO statistics
    if (msg->arrivedOn("bufferIn")) {
        bufferMessageCounter++;
        if (idle) {
            requestFrame();
        }
    } else if (msg->arrivedOn("in")) {
        DataFrame *frame = check_and_cast<DataFrame *>(msg);
        currentFrameID = frame->getId();
        bufferMessageCounter--;
        startWorkOnFrame(0);
    } else if (msg->isSelfMessage()) {
        Buffer *buffer = (Buffer*) (getParentModule()->getSubmodule("inBuffer"));
        buffer->deleteFrame(currentFrameID);
        if (bufferMessageCounter > 0) {
            requestFrame();
        } else {
            idle = true;
        }
    }
    delete msg;
}

void CanTrafficSinkApp::requestFrame() {
    Buffer *buffer = (Buffer*) (getParentModule()->getSubmodule("inBuffer"));
    buffer->deliverNextFrame();
    idle = false;
}

void CanTrafficSinkApp::startWorkOnFrame(double workTime) {
    cMessage *msg = new cMessage("workFinished");
    scheduleAt(simTime() + workTime, msg);
}
