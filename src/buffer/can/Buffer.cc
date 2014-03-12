#include "Buffer.h"

void Buffer::initialize(){
    MOB = par("MOB");
}

void Buffer::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("in")) {
        CanDataFrame *frame = check_and_cast<CanDataFrame *>(msg);
        putFrame(frame);
    }
}

void Buffer::registerDestinationGate() {
    cStringTokenizer destinationGatesTokenizer(
            getParentModule()->par("destinationGates"), ",");
    while (destinationGatesTokenizer.hasMoreTokens()) {
        destinationGates.push_back(
                (cGate *) getParentModule()->getSubmodule("sinkApp")->gate(
                        destinationGatesTokenizer.nextToken()));
    }
}

CanDataFrame* Buffer::getFrame(int id) {
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

void Buffer::deleteFrame(int id) {
    Enter_Method_Silent();
    CanDataFrame *tmp = getFrame(id);
    frames.remove(tmp);
    delete tmp;
}

void Buffer::deliverFrame(int id) {
    Enter_Method_Silent();
    sendToDestinationGates(getFrame(id)->dup());
}

void Buffer::deliverPrioFrame() {
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

void Buffer::deliverNextFrame() {
    Enter_Method_Silent();
    sendToDestinationGates(frames.front()->dup());
}

void Buffer::sendToDestinationGates(CanDataFrame *df) {
    send(df,"out");
}
