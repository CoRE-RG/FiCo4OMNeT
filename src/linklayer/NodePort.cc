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

#include "NodePort.h"

namespace FiCo4OMNeT {

Define_Module(NodePort);

void NodePort::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("phygate$i")) {
        send(msg, "upperLayerOut");
    } else {
        send(msg, "phygate$o");
    }
}

void NodePort::sendMsgToBus(cMessage *msg){ //TODO needed?
    Enter_Method_Silent();
    take(msg);
    send(msg, "phygate$o");
}

}
