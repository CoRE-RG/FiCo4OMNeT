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
//        DataFrame* tmp = check_and_cast<DataFrame *>(*it);
        CanDataFrame* tmp = *it;
        int i = tmp->getCanID();
//        if (tmp->getCanId() == id) {
            if ((i == id)) {
            return tmp;
        }
    }
    return NULL;
}

//void Buffer::putFrame(DataFrame* frame) {
//    frames.push_back(frame);
//    for (std::list<cGate*>::iterator it = destinationGates.begin();
//            it != destinationGates.end(); ++it) {
//        cGate *tmpGate = *it;
//        cMessage *msg = new cMessage("Message in buffer");
//        sendDirect(msg, tmpGate->getOwnerModule()->gate("bufferIn"));
//    }
//}

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
//        if ((tmp->getCanID() < prioId)) {
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
//    for (std::list<cGate*>::iterator it = destinationGates.begin();
//            it != destinationGates.end(); ++it) {
//        cGate *tmpGate = *it;
//        sendDirect(df->dup(), tmpGate);
//    }
    send(df,"out");
}
