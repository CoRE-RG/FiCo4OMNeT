//Copyright (c) 2014, CoRE Research Group, Hamburg University of Applied Sciences
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification,
//are permitted provided that the following conditions are met:
//
//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
//3. Neither the name of the copyright holder nor the names of its contributors
//   may be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
//ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "FiCo4OMNeT_CanOutputBuffer.h"

namespace FiCo4OMNeT {

Define_Module(CanOutputBuffer);

CanOutputBuffer::~CanOutputBuffer(){
    for (std::list<CanDataFrame*>::iterator it =  frames.begin(); it != frames.end(); ++it)
    {
            cancelAndDelete((*it));
    }
    frames.clear();
}

void CanOutputBuffer::putFrame(cMessage* msg) {
    CanDataFrame *frame = dynamic_cast<CanDataFrame *>(msg);
    if (MOB == true) {
        if (getFrame(frame->getCanID()) != NULL) {
            checkoutFromArbitration(frame->getCanID());
        }
    }
    frames.push_back(frame);
    registerForArbitration(frame->getCanID(), frame->getRtr());
    emit(rxPkSignal, msg);
}

void CanOutputBuffer::registerForArbitration(unsigned int canID, bool rtr) {
    CanBusLogic *canBusLogic =
            dynamic_cast<CanBusLogic*> (getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->getSubmodule(
                    "canBusLogic"));
    canBusLogic->registerForArbitration(canID, this, simTime(), rtr);
}

void CanOutputBuffer::checkoutFromArbitration(unsigned int canID) {
    CanBusLogic *canBusLogic =
            dynamic_cast<CanBusLogic*> (getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->getSubmodule(
                    "canBusLogic"));
    if (canBusLogic->getCurrentSendingId() != canID && canBusLogic->getSendingNodeID() != this->getId()) {
        canBusLogic->checkoutFromArbitration(canID);
        deleteFrame(canID);
    }
}

void CanOutputBuffer::receiveSendingPermission(unsigned int canID) {
    Enter_Method_Silent
    ();
    deliverFrame(canID);
}

void CanOutputBuffer::sendingCompleted() {
    Enter_Method_Silent
    ();
    deleteFrame(currentFrame);
    currentFrame = NULL;
    CanPortOutput* portOutput = check_and_cast<CanPortOutput*>(
            getParentModule()->getSubmodule("canNodePort")->getSubmodule(
                    "canPortOutput"));
    portOutput->sendingCompleted();
}

}
