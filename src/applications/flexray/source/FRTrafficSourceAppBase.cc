//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "FRTrafficSourceAppBase.h"

void FRTrafficSourceAppBase::initialize() {
    //static frames in Buffer & in Liste
    //dynamic frames in Liste
    getParentModule()->subscribe("newCycle", this);
    setUpStaticFrames();
}

void FRTrafficSourceAppBase::handleMessage(cMessage *msg) {
    //alle static frames + random (?) dynamic frames
}

void FRTrafficSourceAppBase::receiveSignal(cComponent *source,
        simsignal_t signalID, long l) {
    //Nachrichten an Buffer bei NEW_CYCLE
    vCycleCounter = l;
    frameGenerationForNewCycle();
}

void FRTrafficSourceAppBase::setUpStaticFrames() {
    FRFrame *frMsg;
    int cycleNr;
    int gNumberOfStaticSlots = getParentModule()->par("gNumberOfStaticSlots");
    int syncFrame = getParentModule()->par("syncFrame");
    std::list<unsigned int> staticSlotsChA;
    std::list<unsigned int> staticSlotsChB;

    const char *slots = par("staticSlotsChA");
    cStringTokenizer tokenizerChA(slots);
    while (tokenizerChA.hasMoreTokens()) {
        staticSlotsChA.push_back(atoi(tokenizerChA.nextToken()));
    }

    slots = par("staticSlotsChB");
    cStringTokenizer tokenizerChB(slots);
    while (tokenizerChB.hasMoreTokens()) {
        staticSlotsChB.push_back(atoi(tokenizerChB.nextToken()));
    }

    while (!staticSlotsChA.empty() || !staticSlotsChB.empty()) {
        frMsg = new FRFrame();
        if ((staticSlotsChA.front() == staticSlotsChB.front())
                && !staticSlotsChA.empty() && !staticSlotsChB.empty()) {
            cycleNr = ceil((staticSlotsChA.front() - 1) / gNumberOfStaticSlots);
            frMsg = createFRFrame(
                    staticSlotsChA.front() - cycleNr * gNumberOfStaticSlots,
                    cycleNr, 2, false, STATIC_EVENT);
            staticSlotsChA.pop_front();
            staticSlotsChB.pop_front();

        } else if ((staticSlotsChA.front() < staticSlotsChB.front()
                && !staticSlotsChA.empty()) || staticSlotsChB.empty()) {
            cycleNr = ceil((staticSlotsChA.front() - 1) / gNumberOfStaticSlots);
            frMsg = createFRFrame(
                    staticSlotsChA.front() - cycleNr * gNumberOfStaticSlots,
                    cycleNr, 0, false, STATIC_EVENT);
            staticSlotsChA.pop_front();
        } else {
            cycleNr = ceil((staticSlotsChB.front() - 1) / gNumberOfStaticSlots);
            frMsg = createFRFrame(
                    staticSlotsChB.front() - cycleNr * gNumberOfStaticSlots,
                    cycleNr, 1, false, STATIC_EVENT);
            staticSlotsChB.pop_front();
        }
        if (frMsg->getFrameID() == syncFrame) {
            frMsg->setSyncFrameIndicator(true);
        }
        outgoingStaticFrames.push_back(frMsg);
    }
}

void FRTrafficSourceAppBase::setUpDynamicFrames() {

    cStringTokenizer tokenizerChA(getParentModule()->par("dynamicSlotsChA"));
    dynamicFrameCreation(tokenizerChA, 0);
    cStringTokenizer tokenizerChB(getParentModule()->par("dynamicSlotsChB"));
    dynamicFrameCreation(tokenizerChB, 1);
}

void FRTrafficSourceAppBase::dynamicFrameCreation(cStringTokenizer tokenizer,
        int channel) {
    int gNumberOfMinislots = getParentModule()->par("gNumberOfMinislots");
    int slot;
    int cycleNr;
    FRFrame *frMsg;
    while (tokenizer.hasMoreTokens()) {
        slot = atoi(tokenizer.nextToken());
        cycleNr = ceil((slot - 1) / gNumberOfMinislots);
        frMsg = createFRFrame(
                getDynamicSlot(slot - cycleNr * gNumberOfMinislots), cycleNr,
                channel, false, DYNAMIC_EVENT);
        outgoingDynamicFrames.push_back(frMsg);
    }
}

FRFrame* FRTrafficSourceAppBase::createFRFrame(int frameID, int cycleNumber,
        int channel, bool syncFrameIndicator, int kind) {
    FRFrame *msg = new FRFrame();
    msg->setFrameID(frameID);
    msg->setCycleNumber(cycleNumber);
    msg->setChannel(channel);
    msg->setSyncFrameIndicator(syncFrameIndicator);
    msg->setKind(kind);
    if (kind == DYNAMIC_EVENT) {
//        msg->setSize(randomSize());
    }
    cPacket *payload = new cPacket();
    payload->setBitLength(msg->getSize());
    msg->encapsulate(payload);
    return msg;
}

int FRTrafficSourceAppBase::getDynamicSlot(int slot) {
    int gNumberOfStaticSlots = getParentModule()->par("gNumberOfStaticSlots");
    return slot + gNumberOfStaticSlots;
}

int FRTrafficSourceAppBase::calculateLength(int dataLength) {
//    int arbFieldLength = 0;
//    if (canVersion.compare("2.0B") == 0) {
//        arbFieldLength += ARBITRATIONFIELD29BIT;
//    }
//    return (arbFieldLength + DATAFRAMECONTROLBITS + (dataLength * 8) + calculateStuffingBits(dataLength, arbFieldLength));
    return 0;
}

void FRTrafficSourceAppBase::frameGenerationForNewCycle() {
    //dynamische frames für den aktuellen zyklus erstellen und an Buffer weiterleiten
    FRFrame *tmp;
    for (std::vector<FRFrame*>::iterator it = outgoingDynamicFrames.begin();
            it != outgoingDynamicFrames.end(); ++it) {
        tmp = *it;
        if (tmp->getCycleNumber() == vCycleCounter) {
            transmitFrame(tmp);
        }
    }
    for (std::vector<FRFrame*>::iterator it = outgoingStaticFrames.begin();
            it != outgoingStaticFrames.end(); ++it) {
        tmp = *it;
        if (tmp->getCycleNumber() == vCycleCounter) {
            transmitFrame(tmp);
        }
    }
}

void FRTrafficSourceAppBase::transmitFrame(FRFrame *frMsg){
    send(frMsg,"out");
}
