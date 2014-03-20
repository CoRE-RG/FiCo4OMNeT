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

#include "FRPortInput.h"

void FRPortInput::initialize() {
    bandwidth = par("asdf");//TODO wo bekomme ich das am besten her?
}

void FRPortInput::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        send(msg, "out");
    } else {

        FRFrame *frMsg = dynamic_cast<FRFrame*>(msg);
        FRScheduler *frScheduler =
                (FRScheduler*) (getParentModule()->getParentModule()->getSubmodule("frScheduler"));
        if (frMsg->getKind() == DYNAMIC_EVENT) {
            frScheduler->dynamicFrameReceived(frMsg->getSize(),
                    frMsg->getChannel());
        } else {
            if (frScheduler->getSlotCounter()
                    == (unsigned int) frMsg->getFrameID()) {
                if (frMsg->getSyncFrameIndicator()) {
                    FRSync *frSync = (FRSync*) (getParentModule()->getParentModule()->getSubmodule(
                            "frSync")); //TODO in init
                    frSync->storeDeviationValue(frMsg->getFrameID(),
                            frMsg->getCycleNumber() % 2, frMsg->getChannel(),
                            frScheduler->calculateDeviationValue(), true);
                }
            } else {
                EV << "received static frame in wrong slot!\n";
                bubble("static frame in wrong slot");
            }
        }
        scheduleAt(simTime()+calculateScheduleTiming(frMsg->getSize()),frMsg);
    }


    // von auﬂen: schedulen bis empfang abgeschlossen
    // wann sync frames an sync?
    // receiveMessage aufrufen
    //
    // self message: an buffer weiterleiten
}

void FRPortInput::receiveDynamicFrame(FRFrame *frMsg){

}

void FRPortInput::receiveStaticFrame(FRFrame *frMsg){

}

void FRPortInput::receiveMessage(FRFrame *msg) {
    // frame schedulen und mit calculate ScheduleTiming dauer berechnen
}

double FRPortInput::calculateScheduleTiming(int length) {
    // zeit bis Empfang abgeschlossen ist berechnens
    return ((double) length) / bandwidth;
}

void FRPortInput::forwardFrame(FRFrame *msg) {

}
