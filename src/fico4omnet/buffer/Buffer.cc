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

#include "fico4omnet/buffer/Buffer.h"
#include "fico4omnet/utilities/HelperFunctions.h"

namespace FiCo4OMNeT {

simsignal_t Buffer::queueLengthSignal = registerSignal("length");
simsignal_t Buffer::queueSizeSignal = registerSignal("size");

Buffer::~Buffer(){
    for (cMessage* element: frames){
        delete element;
    }
}
void Buffer::initialize() {
    initializeStatistics();
    registerDestinationGate();
    queueSize = 0;
}

void Buffer::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("in") || msg->arrivedOn("directIn")) {
        recordPacketReceived(msg);
        putFrame(msg);
    }
}

void Buffer::registerDestinationGate() {
    destinationGates.clear();
    std::vector<std::string> destinationGatePaths = cStringTokenizer(
            par("destination_gates").stringValue(), ",").asVector();
    for (std::vector<std::string>::const_iterator destinationGatePath =
            destinationGatePaths.begin();
            destinationGatePath != destinationGatePaths.end();
            ++destinationGatePath) {

        cGate* destinationGate = gateByFullPath((*destinationGatePath));
        if (!destinationGate)
        {
            destinationGate = gateByShortPath((*destinationGatePath), this);
        }
        if (destinationGate) {
            destinationGates.push_back(destinationGate);
        } else {
            throw cRuntimeError(
                    "Configuration problem of destination_gates: Gate: %s could not be resolved!",
                    (*destinationGatePath).c_str());
        }
    }
}

cMessage* Buffer::getFrame(long objectId) {
    for (std::list<cMessage*>::iterator it = frames.begin(); it != frames.end();
            ++it) {
        cMessage* tmp = *it;
        long i = tmp->getId();
        if (i == objectId) {
            return tmp;
        }
    }
    return nullptr;
}

void Buffer::putFrame(cMessage* frame) {
    frames.push_back(frame);
    emit(queueLengthSignal, static_cast<unsigned long>(frames.size()));
    if(cPacket* packet = dynamic_cast<cPacket*>(frame))
    {
        queueSize+=static_cast<size_t>(packet->getByteLength());
    }
    emit(queueSizeSignal, static_cast<unsigned long>(queueSize));
}

void Buffer::deleteFrame(int objectId) {
    Enter_Method_Silent
    ();
    cMessage *tmp = getFrame(objectId);
    frames.remove(tmp);
    emit(queueLengthSignal, static_cast<unsigned long>(frames.size()));
    if(cPacket* packet = dynamic_cast<cPacket*>(tmp))
    {
        queueSize-=static_cast<size_t>(packet->getByteLength());
    }
    emit(queueSizeSignal, static_cast<unsigned long>(queueSize));
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
