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

#include "FiCo4OMNeT_FRPort.h"

namespace FiCo4OMNeT {

//Define_Module(FRPort);

void FRPort::handleMessage(cMessage *msg) {
	take(msg);
	FRFrame *frMsg = dynamic_cast<FRFrame*> (msg);
	FRScheduler *frScheduler = dynamic_cast<FRScheduler*> (getParentModule()->getSubmodule(
			"frScheduler"));
	if (frMsg->getKind() == DYNAMIC_EVENT) {
		frScheduler->dynamicFrameReceived(frMsg->getByteLength(), static_cast<unsigned int> (frMsg->getChannel()));
		//ev << frMsg->getFrameID() << "," << simTime() << "," << simTime()-msg->getCreationTime() << endl;
	} else {
		if (frScheduler->getSlotCounter() == static_cast<unsigned int> (frMsg->getFrameID())){
			if (frMsg->getSyncFrameIndicator()) {
				FRSync *frSync = dynamic_cast<FRSync*> (getParentModule()->getSubmodule(
						"frSync"));
				frSync->storeDeviationValue(frMsg->getFrameID(),
						frMsg->getCycleNumber() % 2, frMsg->getChannel(),
						frScheduler->calculateDeviationValue(), true);
			}
		} else {
			EV << "received static frame in wrong slot!\n";
			bubble("static frame in wrong slot");
		}
	}
	FRApp *frApp = dynamic_cast<FRApp*> (getParentModule()->getSubmodule(par("appName").stringValue()));
	sendDirect(frMsg,frApp->gate("frameIn"));
}

void FRPort::sendMsg(FRFrame *msg) {
	Enter_Method_Silent();
	take(msg);
	if (msg->getChannel() == 0) {
		send(msg, "phyChannelA$o");
	} else if (msg->getChannel() == 1) {
		send(msg, "phyChannelB$o");
	}

}

}
