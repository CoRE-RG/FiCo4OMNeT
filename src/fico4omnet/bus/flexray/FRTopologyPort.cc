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

#include "fico4omnet/bus/flexray/FRTopologyPort.h"

namespace FiCo4OMNeT {

Define_Module(FRTopologyPort);

void FRTopologyPort::handleMessage(cMessage *msg) {
	if (msg->arrivedOn("phyChannelA$i")) {
		for (int i = 0; i < this->gateSize("phyChannelA"); ++i) {
			if (i != msg->getArrivalGate()->getIndex()) {
				cMessage *newMsg = msg->dup();
				send(newMsg, "phyChannelA$o", i);
			}
		}
	} else if (msg->arrivedOn("phyChannelB$i")) {
		for (int i = 0; i < this->gateSize("phyChannelB"); ++i) {
			if (i != msg->getArrivalGate()->getIndex()) {
				cMessage *newMsg = msg->dup();
				send(newMsg, "phyChannelB$o", i);
			}
		}
	}
	delete msg;
}

}
