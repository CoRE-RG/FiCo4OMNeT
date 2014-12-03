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

#include "FiCo4OMNeT_Buffer.h"
#include "FiCo4OMNeT_HelperFunctions.h"

namespace FiCo4OMNeT {

void Buffer::initialize() {
    initializeStatistics();
    registerDestinationGate();

}

void Buffer::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("in") || msg->arrivedOn("directIn")) {
        recordPacketReceived(msg);
        putFrame(msg);
    }
}

void Buffer::registerDestinationGate() { //TODO needs upgrade for multiple sink apps
//    cStringTokenizer destinationGatesTokenizer(
//            getParentModule()->par("destinationGates"), ",");
//    while (destinationGatesTokenizer.hasMoreTokens()) {
//        destinationGates.push_back(
//                (cGate *) getParentModule()->getSubmodule("sinkApp")->gate(
//                        destinationGatesTokenizer.nextToken()));
//    }

    destinationGates.clear();
    std::vector<std::string> destinationGatePaths = cStringTokenizer(
            par("destination_gates").stringValue(), ",").asVector();
    for (std::vector<std::string>::const_iterator destinationGatePath =
            destinationGatePaths.begin();
            destinationGatePath != destinationGatePaths.end();
            destinationGatePath++) {

        cGate* gate = gateByFullPath((*destinationGatePath));
        if (!gate)
        {
            gate = gateByShortPath((*destinationGatePath), this);
        }
        if (gate) {
//            if (findContainingNode(gate->getOwnerModule()) != findContainingNode(this))
//            {
//                throw cRuntimeError(
//                        "Configuration problem of destination_gates: Gate: %s is not in node %s! Maybe a copy-paste problem?",
//                        (*destinationGatePath).c_str(), findContainingNode(this)->getFullName());
//            }
            destinationGates.push_back(gate);
        } else {
            throw cRuntimeError(
                    "Configuration problem of destination_gates: Gate: %s could not be resolved!",
                    (*destinationGatePath).c_str());
        }
    }
}

cMessage* Buffer::getFrame(int objectId) {
    for (std::list<cMessage*>::iterator it = frames.begin(); it != frames.end();
            ++it) {
        cMessage* tmp = *it;
        int i = tmp->getId();
        if ((i == objectId)) {
            return tmp;
        }
    }
    return NULL;
}

void Buffer::putFrame(cMessage* frame) {
    frames.push_back(frame);
}

void Buffer::deleteFrame(int objectId) {
    Enter_Method_Silent
    ();
    cMessage *tmp = getFrame(objectId);
    frames.remove(tmp);
    delete tmp;
}

void Buffer::deliverFrame(int id) {
    Enter_Method_Silent
    ();
    sendToDestinationGates(getFrame(id)->dup());
}

void Buffer::deliverNextFrame() {
    Enter_Method_Silent
    ();
    sendToDestinationGates(frames.front()->dup());
}

void Buffer::sendToDestinationGates(cMessage *df) {
    recordPacketSent(df);

    for (std::list<cGate*>::const_iterator dgate = destinationGates.begin();
            dgate != destinationGates.end(); ++dgate) {
        sendDirect(df->dup(), 0, 0, *dgate);
    }
    if (gate("out")->isConnected()) {
        send(df->dup(), "out");
    }

    delete df;
}

void Buffer::initializeStatistics() {
    txPkSignal = registerSignal("txPk");
    rxPkSignal = registerSignal("rxPk");
}

void Buffer::recordPacketSent(cMessage *frame) {
    emit(txPkSignal, frame);
}

void Buffer::recordPacketReceived(cMessage *frame) {
    emit(rxPkSignal, frame);
}

}
