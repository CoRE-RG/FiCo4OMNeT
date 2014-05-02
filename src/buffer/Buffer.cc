#include "Buffer.h"

namespace FiCo4OMNeT {

//Define_Module(Buffer);

void Buffer::initialize(){
}

void Buffer::finish(){
    frames.clear();
}

void Buffer::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("in")) {
        putFrame(msg);
    }
}

void Buffer::registerDestinationGate() { //TODO mal schauen ob das n�tig ist
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
    send(df,"out");
}

}
