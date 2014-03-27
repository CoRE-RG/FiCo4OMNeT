#include "FRBuffer.h"

void FRBuffer::initialize(){
}

void FRBuffer::registerDestinationGate() {
    cStringTokenizer destinationGatesTokenizer(
            getParentModule()->par("destinationGates"), ",");
    while (destinationGatesTokenizer.hasMoreTokens()) {
        destinationGates.push_back(
                (cGate *) getParentModule()->getSubmodule("sinkApp")->gate(
                        destinationGatesTokenizer.nextToken()));
    }
}

FRFrame* FRBuffer::getFrame(int id) {
    for (std::list<FRFrame*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        FRFrame* tmp = *it;
        int i = tmp->getId();
            if ((i == id)) {
            return tmp;
        }
    }
    return NULL;
}

void FRBuffer::putFrame(cMessage* msg){
    FRFrame *frame = dynamic_cast<FRFrame *>(msg);
    frames.push_back(frame);
}

void FRBuffer::deleteFrame(int id) {
    Enter_Method_Silent();
    FRFrame *tmp = getFrame(id);
    frames.remove(tmp);
    delete tmp;
}

void FRBuffer::deliverFrame(int id) {
    Enter_Method_Silent();
    sendToDestinationGates(getFrame(id)->dup());
}

void FRBuffer::deliverPrioFrame() {
    Enter_Method_Silent();
    int prioId = INT_MAX;
    FRFrame *prioFrame;
    for (std::list<FRFrame*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        FRFrame *tmp = *it;
        int i = tmp->getId();
        if ((i < prioId)) {
            prioFrame = tmp;
            prioId = i;
        }
    }
    sendToDestinationGates(prioFrame->dup());
}

void FRBuffer::deliverNextFrame() {
    Enter_Method_Silent();
    sendToDestinationGates(frames.front()->dup());
}

void FRBuffer::sendToDestinationGates(FRFrame *df) {
    send(df,"out");
}
