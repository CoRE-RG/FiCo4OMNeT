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

#include "OutputBuffer.h"

void OutputBuffer::putFrame(DataFrame* frame) {
    EV <<"1.1\n";
    frames.push_back(frame);
    EV <<"1.2\n";
    registerForArbitration(frame->getID(), frame->getRtr());
    EV <<"1.3\n";
}

void OutputBuffer::registerForArbitration(int id, bool rtr) {
    EV <<"2.1\n";
//    cModule *mod =  simulation.getModuleByPath("NewLayout.bus.canBusApp");
//    CanTrafficSourceApp *source = (CanTrafficSourceApp*) (getParentModule()->getSubmodule("sourceApp"));
    CanBusApp *canBusApp =(CanBusApp*)(simulation.getModuleByPath("NewLayout.bus.canBusApp"));
//            (CanBusApp*) (getParentModule()->getParentModule()->getSubmodule(
//                    "canBus")->getSubmodule("canBusApp"));
    EV <<"2.2\n";
    canBusApp->registerForArbitration(id, this, simTime(), rtr);
    EV <<"2.3\n";
}

void OutputBuffer::receiveSendingPermission(int id) {
    deliverFrame(id);
}

void OutputBuffer::sendingCompleted(int id) {
    deleteFrame(id);
}
