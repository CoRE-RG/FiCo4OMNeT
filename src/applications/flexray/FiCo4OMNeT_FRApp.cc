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

#include "FiCo4OMNeT_FRApp.h"

namespace FiCo4OMNeT {

Define_Module(FRApp);

void FRApp::initialize() {
	//Register schedulerIn gate
	FRScheduler *frScheduler = (FRScheduler*) (getParentModule()->getSubmodule(
			"frScheduler"));
	frScheduler->setFRAppGate(gate("schedulerIn"));
}

void FRApp::handleMessage(cMessage *msg) {
	if (msg->arrivedOn("schedulerIn")
			&& (msg->getKind() == STATIC_EVENT || DYNAMIC_EVENT)) {
		FRScheduler *frScheduler =
				(FRScheduler*) (getParentModule()->getSubmodule("frScheduler"));
		SchedulerActionTimeEvent *event = (SchedulerActionTimeEvent*) msg;
		FRPort *frPort = (FRPort*) (getParentModule()->getSubmodule("frPort"));
		if (msg->getKind() == STATIC_EVENT) {
			if (event->getSyncFrameIndicator()) {
				FRSync *frSync = (FRSync*) (getParentModule()->getSubmodule(
						"frSync"));
				frSync->storeOwnSyncFrame(event->getFrameID(),
						frScheduler->getCycleCounter() % 2);
			}
			if (event->getChannel() == 2) {
				frPort->sendMsg(
						createFRFrame(event->getFrameID(),
								frScheduler->getCycleCounter(), 0,
								event->getSyncFrameIndicator(), STATIC_EVENT));
				frPort->sendMsg(
						createFRFrame(event->getFrameID(),
								frScheduler->getCycleCounter(), 1,
								event->getSyncFrameIndicator(), STATIC_EVENT));
			} else {
				frPort->sendMsg(
						createFRFrame(event->getFrameID(),
								frScheduler->getCycleCounter(),
								event->getChannel(),
								event->getSyncFrameIndicator(), STATIC_EVENT));
			}
		} else {
			frPort->sendMsg(
					createFRFrame(event->getFrameID(),
							frScheduler->getCycleCounter(), event->getChannel(),
							event->getSyncFrameIndicator(), DYNAMIC_EVENT));
		}
		delete msg;
	} else {
	    delete msg;
	}
}



FRFrame* FRApp::createFRFrame(int frameID, int cycleNumber, int channel,
		bool syncFrameIndicator, int kind) {
	FRFrame *msg = new FRFrame();
	msg->setFrameID(frameID);
	msg->setCycleNumber(cycleNumber);
	msg->setChannel(channel);
	msg->setSyncFrameIndicator(syncFrameIndicator);
	msg->setKind(kind);
	if (kind == DYNAMIC_EVENT) {
        msg->setSize(randomSize());
//        if (channel == 0) {
//            std::cout << "ID" << msg->getFrameID() << "," << simTime() << "," << simTime()-lastcreate << endl;
//            lastcreate = simTime();
//        }
    }
	cPacket *payload = new cPacket();
	payload->setBitLength(msg->getSize());
	msg->encapsulate(payload);
	return msg;
}

void FRApp::setMaxRandom(int max){
    maxRandom = max;
}

int FRApp::randomSize(){
    return intuniform(0,maxRandom);
}

}
