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

#include "busPort.h"

//namespace std {
//Define_Module(busPort);

void busPort::handleMessage(cMessage *msg) {
    take(msg);
    canBusApp *canbus = (canBusApp*) (getParentModule()->getSubmodule("canBusApp"));
    sendDirect(msg,canbus->gate("frameIn"));
}

void busPort::forward_to_all(cMessage *msg) {
    for (int i = 0; i < this->gateSize("phyChannelA"); ++i) {
        cMessage *newMsg = msg->dup();
        send(newMsg, "phyChannelA$o", i);
    }
    delete msg;
}

void busPort::sendMsg(cMessage *msg, int id){
    send(msg, "gate$o", id);
}
//} /* namespace flexray */
