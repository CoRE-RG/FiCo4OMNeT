#include "Buffer.h"

namespace FiCo4OMNeT {

void Buffer::initialize(){
    initializeStatistics();
}

void Buffer::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("in")) {
        putFrame(msg);
        recordPacketReceived(msg);
    }
}

void Buffer::registerDestinationGate() { //TODO needs upgrade for multiple sink apps
    cStringTokenizer destinationGatesTokenizer(
            getParentModule()->par("destinationGates"), ",");
    while (destinationGatesTokenizer.hasMoreTokens()) {
        destinationGates.push_back(
                (cGate *) getParentModule()->getSubmodule("sinkApp")->gate(
                        destinationGatesTokenizer.nextToken()));
    }
}

cMessage* Buffer::getFrame(int objectId) {
    for (std::list<cMessage*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        cMessage* tmp = *it;
        int i = tmp->getId();
            if ((i == objectId)) {
            return tmp;
        }
    }
    return NULL;
}

void Buffer::putFrame(cMessage* frame){
    frames.push_back(frame);
}

void Buffer::deleteFrame(int objectId) {
    Enter_Method_Silent();
    cMessage *tmp = getFrame(objectId);
    frames.remove(tmp);
    delete tmp;
}

void Buffer::deliverFrame(int id) {
    Enter_Method_Silent();
    sendToDestinationGates(getFrame(id)->dup());
}

void Buffer::deliverNextFrame() {
    Enter_Method_Silent();
    sendToDestinationGates(frames.front()->dup());
}

void Buffer::sendToDestinationGates(cMessage *df) {
    recordPacketSent(df);
    send(df,"out");
}

void Buffer::initializeStatistics()
{
    txPkSignal = registerSignal("txPk");
    rxPkSignal = registerSignal("rxPk");
}

void Buffer::recordPacketSent(cMessage *frame)
{
    emit(txPkSignal, frame);
}

void Buffer::recordPacketReceived(cMessage *frame)
{
    emit(rxPkSignal, frame);
}

}
