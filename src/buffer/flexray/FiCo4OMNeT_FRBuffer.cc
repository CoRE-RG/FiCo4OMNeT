#include "FiCo4OMNeT_FRBuffer.h"

namespace FiCo4OMNeT {

void FRBuffer::initialize(){
    Buffer::initialize();
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

FRFrame* FRBuffer::getFrame(int frameId) {
    for (std::list<FRFrame*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        FRFrame* tmp = *it;
        int i = tmp->getFrameID();
            if ((i == frameId)) {
            return tmp;
        }
    }
    return NULL;
}

void FRBuffer::putFrame(cMessage* msg){
    FRFrame *frame = dynamic_cast<FRFrame *>(msg);
    frames.push_back(frame);
}

void FRBuffer::deleteFrame(int frameId) {
    Enter_Method_Silent();
    FRFrame *tmp = getFrame(frameId);
    frames.remove(tmp);
    delete tmp;
}

void FRBuffer::deliverFrame(int frameId) {
    Enter_Method_Silent();
    FRFrame *tmp = getFrame(frameId);
    if (tmp != NULL) {
        sendToDestinationGates(tmp->dup());
        deleteFrame(frameId);
    }
}

void FRBuffer::deliverPrioFrame() {
    Enter_Method_Silent();
    int prioId = INT_MAX;
    FRFrame *prioFrame;
    for (std::list<FRFrame*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        FRFrame *tmp = *it;
        int i = tmp->getFrameID();
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
    recordPacketSent(df);
}

}
