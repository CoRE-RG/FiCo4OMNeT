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

#include "CanOutputBuffer.h"

namespace FiCo4OMNeT {

Define_Module(CanOutputBuffer);

void CanOutputBuffer::putFrame(cMessage* msg) {
    CanDataFrame *frame = dynamic_cast<CanDataFrame *>(msg);
    if (MOB == true) {
        if (getFrame(frame->getCanID()) != NULL) {
            deleteFrame(frame->getCanID());
            checkoutFromArbitration(frame->getCanID());
        }
    }
    frames.push_back(frame);
    registerForArbitration(frame->getCanID(), frame->getRtr());
}

void CanOutputBuffer::registerForArbitration(int id, bool rtr) {
    CanBusLogic *canBusLogic =
            (CanBusLogic*) (getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->getSubmodule(
                    "canBusLogic"));
    canBusLogic->registerForArbitration(id, this, simTime(), rtr);
}

void CanOutputBuffer::checkoutFromArbitration(int id) {
    CanBusLogic *canBusLogic =
            (CanBusLogic*) (getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->getSubmodule(
                    "canBusLogic"));
    canBusLogic->checkoutFromArbitration(id);

}

void CanOutputBuffer::receiveSendingPermission(int id) {
    Enter_Method_Silent
    ();
    deliverFrame(id);
}

void CanOutputBuffer::sendingCompleted(int id) {
    Enter_Method_Silent
    ();
    deleteFrame(id);
    CanPortOutput* portOutput = check_and_cast<CanPortOutput*>(
            getParentModule()->getSubmodule("canNodePort")->getSubmodule(
                    "canPortOutput"));
    portOutput->sendingCompleted();
}

}
