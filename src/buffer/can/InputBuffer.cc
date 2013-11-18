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

#include "InputBuffer.h"

void InputBuffer::putFrame(CanDataFrame* frame) {
    frames.push_back(frame);
    //TODO weiterleiten an sinkApp
//    send(new cMessage("Message in buffer"),"out");

//    sendDirect(msg,getParentModule()->getSubmodule("sinkApp"), "controllerIn");
//    for (std::list<cGate*>::iterator it = destinationGates.begin();
//            it != destinationGates.end(); ++it) {
//        cGate *tmpGate = *it;
//        cMessage *msg = new cMessage("Message in buffer");
//        sendDirect(msg, tmpGate->getOwnerModule()->gate("bufferIn"));
//    }
}
