#include "CanBuffer.h"

void CanBuffer::initialize(){
    MOB = par("MOB");
}

void CanBuffer::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("in")) {
        CanDataFrame *frame = check_and_cast<CanDataFrame *>(msg);
        putFrame(frame);
    }
}

void CanBuffer::registerDestinationGate() {
    cStringTokenizer destinationGatesTokenizer(
            getParentModule()->par("destinationGates"), ",");
    while (destinationGatesTokenizer.hasMoreTokens()) {
        destinationGates.push_back(
                (cGate *) getParentModule()->getSubmodule("sinkApp")->gate(
                        destinationGatesTokenizer.nextToken()));
    }
}

CanDataFrame* CanBuffer::getFrame(int id) {
    for (std::list<CanDataFrame*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        CanDataFrame* tmp = *it;
        int i = tmp->getCanID();
            if ((i == id)) {
            return tmp;
        }
    }
    return NULL;
}

void CanBuffer::putFrame(CanDataFrame* frame){
    frames.push_back(frame);
}

void CanBuffer::deleteFrame(int id) {
    Enter_Method_Silent();
    CanDataFrame *tmp = getFrame(id);
    frames.remove(tmp);
    delete tmp;
}

void CanBuffer::deliverFrame(int id) {
    Enter_Method_Silent();
    sendToDestinationGates(getFrame(id)->dup());
}

void CanBuffer::deliverPrioFrame() {
    Enter_Method_Silent();
    int prioId = INT_MAX;
    CanDataFrame *prioFrame;
    for (std::list<CanDataFrame*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        CanDataFrame *tmp = *it;
        int i = tmp->getCanID();
        if ((i < prioId)) {
            prioFrame = tmp;
            prioId = i;
        }
    }
    sendToDestinationGates(prioFrame->dup());
}

void CanBuffer::deliverNextFrame() {
    Enter_Method_Silent();
    sendToDestinationGates(frames.front()->dup());
}

void CanBuffer::sendToDestinationGates(CanDataFrame *df) {
    send(df,"out");
}
