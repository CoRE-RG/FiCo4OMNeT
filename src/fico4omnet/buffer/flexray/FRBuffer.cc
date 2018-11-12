#include "fico4omnet/buffer/flexray/FRBuffer.h"

namespace FiCo4OMNeT {

void FRBuffer::initialize(){
    Buffer::initialize();
}

//void FRBuffer::registerDestinationGate() {
//    cStringTokenizer destinationGatesTokenizer(
//            getParentModule()->par("destinationGates"), ",");
//    while (destinationGatesTokenizer.hasMoreTokens()) {
//        destinationGates.push_back(
//                (cGate *) getParentModule()->getSubmodule("sinkApp")->gate(
//                        destinationGatesTokenizer.nextToken()));
//    }
//}

FRFrame* FRBuffer::getFrame(int frameId) {
    for (std::list<cMessage*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        FRFrame* tmp = dynamic_cast<FRFrame*>(*it);
        int i = tmp->getFrameID();
            if (i == frameId) {
            return tmp;
        }
    }
    return nullptr;
}

void FRBuffer::putFrame(cMessage* msg){
    FRFrame *frame = dynamic_cast<FRFrame *>(msg);
    frames.push_back(frame);
    emit(queueLengthSignal, static_cast<unsigned long>(frames.size()));
    queueSize+=static_cast<size_t>(frame->getByteLength());
    emit(queueSizeSignal, static_cast<unsigned long>(queueSize));
}

void FRBuffer::deleteFrame(int frameId) {
    Enter_Method_Silent();
    FRFrame *tmp = getFrame(frameId);
    frames.remove(tmp);
    emit(queueLengthSignal, static_cast<unsigned long>(frames.size()));
    queueSize-=static_cast<size_t>(tmp->getByteLength());
    emit(queueSizeSignal, static_cast<unsigned long>(queueSize));
    delete tmp;
}

void FRBuffer::deliverFrame(int frameId) {
    Enter_Method_Silent();
    FRFrame *tmp = getFrame(frameId);
    if (tmp != nullptr) {
        sendToDestinationGates(tmp->dup());
        deleteFrame(frameId);
    }
}

void FRBuffer::deliverPrioFrame() {
    Enter_Method_Silent();
    int prioId = INT_MAX;
    FRFrame *prioFrame = nullptr;
    for (std::list<cMessage*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        FRFrame *tmp = dynamic_cast<FRFrame*>(*it);
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

//void FRBuffer::sendToDestinationGates(FRFrame *df) {
//    send(df,"out");
//    recordPacketSent(df);
//}

}
