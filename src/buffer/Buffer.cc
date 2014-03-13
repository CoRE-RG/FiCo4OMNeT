#include "Buffer.h"

void Buffer::initialize(){
}

void Buffer::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("in")) {
        putFrame(msg);
    }
}

void Buffer::registerDestinationGate() { //TODO mal schauen ob das nötig ist
    cStringTokenizer destinationGatesTokenizer(
            getParentModule()->par("destinationGates"), ",");
    while (destinationGatesTokenizer.hasMoreTokens()) {
        destinationGates.push_back(
                (cGate *) getParentModule()->getSubmodule("sinkApp")->gate(
                        destinationGatesTokenizer.nextToken()));
    }
}

cMessage* Buffer::getFrame(int id) {
//    for (std::list<CanDataFrame*>::iterator it = frames.begin();
//            it != frames.end(); ++it) {
//        CanDataFrame* tmp = *it;
//        int i = tmp->getCanID();
//            if ((i == id)) {
//            return tmp;
//        }
//    }
    return NULL;
}

void Buffer::deleteFrame(int id) {
    Enter_Method_Silent();
//    CanDataFrame *tmp = getFrame(id);
//    frames.remove(tmp);
//    delete tmp;
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
