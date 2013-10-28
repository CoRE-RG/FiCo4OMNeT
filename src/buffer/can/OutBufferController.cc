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

#include "OutBufferController.h"

void OutBufferController::registerForArbitration(int id, bool rtr, bool remoteSent){
    canBusApp *canBus = (canBusApp*) (getParentModule()->getParentModule()->getSubmodule("canBus")->getSubmodule("busPort"));
    canBus->registerForArbitration(id, this, simTime(), rtr, remoteSent);
}

void OutBufferController::receiveSendingPermission(int id){
    Buffer *buf = (Buffer*) (getParentModule()->getSubmodule("outBuffer"));
    buf->deliverFrame(id);
}

void OutBufferController::sendingCompleted(int id){
    Buffer *buf = (Buffer*) (getParentModule()->getSubmodule("outBuffer"));
    buf->deleteFrame(id);
}
