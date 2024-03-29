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

#include "fico4omnet/applications/flexray/source/FRTrafficSourceAppBase.h"

#include "fico4omnet/scheduler/flexray/SchedulerMessage_m.h"

namespace FiCo4OMNeT {

Define_Module(FRTrafficSourceAppBase);

FRTrafficSourceAppBase::FRTrafficSourceAppBase() {
    this->vCycleCounter = 0;

}

void FRTrafficSourceAppBase::initialize() {
    getParentModule()->subscribe("newCycle", this);
//    subscribe("newCycle", this);
    setUpStaticFrames();
    setUpDynamicFrames();
}

void FRTrafficSourceAppBase::handleMessage(omnetpp::cMessage *msg) {
    (void) msg;
    //alle static frames + random (?) dynamic frames
}

void FRTrafficSourceAppBase::receiveSignal(omnetpp::cComponent *source,
        omnetpp::simsignal_t signalID, long l,
        __attribute__((unused))  omnetpp::cObject *details) {
    (void) source;
    (void) signalID;
    //Nachrichten an Buffer bei NEW_CYCLE
    vCycleCounter = static_cast<int>(l);
    frameGenerationForNewCycle();
}

void FRTrafficSourceAppBase::setUpStaticFrames() {
    int cycleNr;
    int gNumberOfStaticSlots = getParentModule()->par("gNumberOfStaticSlots");
    int syncFrame = par("syncFrame");
    std::list<int> staticSlotsChA;
    std::list<int> staticSlotsChB;

    const char *slots = par("staticSlotsChA");
    omnetpp::cStringTokenizer tokenizerChA(slots);
    while (tokenizerChA.hasMoreTokens()) {
        staticSlotsChA.push_back(atoi(tokenizerChA.nextToken()));
    }

    slots = par("staticSlotsChB");
    omnetpp::cStringTokenizer tokenizerChB(slots);
    while (tokenizerChB.hasMoreTokens()) {
        staticSlotsChB.push_back(atoi(tokenizerChB.nextToken()));
    }

    while (!staticSlotsChA.empty() || !staticSlotsChB.empty()) {
        FRFrame * frMsg = new FRFrame();
        if ((staticSlotsChA.front() == staticSlotsChB.front())
                && !staticSlotsChA.empty() && !staticSlotsChB.empty()) {
            cycleNr = static_cast<int>(ceil(
                    (staticSlotsChA.front() - 1) / gNumberOfStaticSlots));
            frMsg = createFRFrame(
                    staticSlotsChA.front() - cycleNr * gNumberOfStaticSlots,
                    cycleNr, CHANNEL_AB, false, STATIC_EVENT);
            staticSlotsChA.pop_front();
            staticSlotsChB.pop_front();

        } else if ((staticSlotsChA.front() < staticSlotsChB.front()
                && !staticSlotsChA.empty()) || staticSlotsChB.empty()) {
            cycleNr = static_cast<int>(ceil(
                    (staticSlotsChA.front() - 1) / gNumberOfStaticSlots));
            frMsg = createFRFrame(
                    staticSlotsChA.front() - cycleNr * gNumberOfStaticSlots,
                    cycleNr, CHANNEL_A, false, STATIC_EVENT);
            staticSlotsChA.pop_front();
        } else {
            cycleNr = static_cast<int>(ceil(
                    (staticSlotsChB.front() - 1) / gNumberOfStaticSlots));
            frMsg = createFRFrame(
                    staticSlotsChB.front() - cycleNr * gNumberOfStaticSlots,
                    cycleNr, CHANNEL_B, false, STATIC_EVENT);
            staticSlotsChB.pop_front();
        }
        if (frMsg->getFrameID() == syncFrame) {
            frMsg->setSyncFrameIndicator(true);
        }
        outgoingStaticFrames.push_back(frMsg);
    }
}

void FRTrafficSourceAppBase::setUpDynamicFrames() {
    dynamicFrameCreation("dynamicSlotsChA", CHANNEL_A);
    dynamicFrameCreation("dynamicSlotsChB", CHANNEL_B);
}

void FRTrafficSourceAppBase::dynamicFrameCreation(const char *dynamicSlotsParName, int channel) {
    omnetpp::cStringTokenizer tokenizer(par(dynamicSlotsParName));
    int gNumberOfMinislots = getParentModule()->par("gNumberOfMinislots");
    while (tokenizer.hasMoreTokens()) {
        int slot = atoi(tokenizer.nextToken());
        int cycleNr = static_cast<int>(ceil((slot - 1) / gNumberOfMinislots));
        FRFrame *frMsg = createFRFrame(
                getDynamicSlot(slot - cycleNr * gNumberOfMinislots), cycleNr,
                channel, false, DYNAMIC_EVENT);
        outgoingDynamicFrames.push_back(frMsg);
    }
}

FRFrame* FRTrafficSourceAppBase::createFRFrame(int frameID, int cycleNumber,
        int channel, bool syncFrameIndicator, short kind) {
    FRFrame *msg = new FRFrame();
    msg->setFrameID(frameID);
    msg->setCycleNumber(cycleNumber);
    msg->setChannel(channel);
    msg->setSyncFrameIndicator(syncFrameIndicator);
    msg->setKind(kind);

    omnetpp::cPacket *payload = new omnetpp::cPacket();
    if (kind == DYNAMIC_EVENT) {
        payload->setByteLength(randomSize());
        EV << "randomsize: " << payload->getByteLength() << "\n";
    } else if (kind == STATIC_EVENT) {
        int staticSlotLength = getParentModule()->par("gPayloadLengthStatic");
        payload->setByteLength(staticSlotLength);
    }
    msg->setPayloadLength((static_cast<int>(payload->getByteLength())));
    payload->setByteLength(msg->getPayloadLength());
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
    return dataLength * 0;
}

void FRTrafficSourceAppBase::frameGenerationForNewCycle() {
    Enter_Method_Silent();
    FRFrame *tmp;
    for (std::vector<FRFrame*>::iterator it = outgoingDynamicFrames.begin();
    it != outgoingDynamicFrames.end(); ++it) {
        tmp = *it;
        if (tmp->getCycleNumber() == vCycleCounter) {
            transmitFrame(tmp->dup());
        }
    }
    for (std::vector<FRFrame*>::iterator it = outgoingStaticFrames.begin();
    it != outgoingStaticFrames.end(); ++it) {
        tmp = *it;
        if (tmp->getCycleNumber() == vCycleCounter) {
            transmitFrame(tmp->dup());
        }
    }
}

int FRTrafficSourceAppBase::randomSize() {
    return 2 * (intuniform(0, getParentModule()->par("cPayloadLengthMax")));
}

void FRTrafficSourceAppBase::transmitFrame(FRFrame *frMsg) {
    send(frMsg, "out");
}

}
