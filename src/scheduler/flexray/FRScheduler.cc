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

#include "FRScheduler.h"

Define_Module( FRScheduler);

simsignal_t FRScheduler::newCycle = SIMSIGNAL_NULL;

void FRScheduler::initialize() {
    gCycleCountMax = par("gCycleCountMax");
    vCycleCounter = gCycleCountMax;
    maxDriftChange = par("maxDriftChange").doubleValue(); //[]
    maxDrift = par("maxDrift").doubleValue(); //[]
    pdMicrotick = par("pdMicrotick").doubleValue(); //[ns]
    gdMacrotick = par("gdMacrotick").doubleValue(); //[µs]
    gdStaticSlot = par("gdStaticSlot"); //[MT]
    gdMinislot = par("gdMinislot"); //[MT]
    gdNIT = par("gdNIT"); //[MT]
    gdSymbolWindow = par("gdSymbolWindow"); //[MT]
    gNumberOfMinislots = par("gNumberOfMinislots");
    gNumberOfStaticSlots = par("gNumberOfStaticSlots");
    gdActionPointOffset = par("gdActionPointOffset"); //[MT]
    gdMinislotActionPointOffset = par("gdMinislotActionPointOffset"); //[MT]
    busSpeed = par("busSpeed").doubleValue();
    syncFrame = par("syncFrame");

    currentTick = pdMicrotick;
    newCycle = registerSignal("newCycle");
    scheduleAt(simTime(), new SchedulerEvent("NEW_CYCLE", NEW_CYCLE));
    lastCycleStart = simTime();
    pMicroPerCycle = (getCycleTicks() * gdMacrotick) / pdMicrotick;

    zRateCorrection = 0;
    zOffsetCorrection = 0;

    FRApp *frApp = (FRApp*) (getParentModule()->getSubmodule("frApp"));
    frApp->setMaxRandom(
            (busSpeed * 1024 * 1024
                    * (gNumberOfMinislots * gdMinislot * gdMacrotick)) / 4);
}

void FRScheduler::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && msg->getKind() == NEW_CYCLE) {
        additionalMinislotsChA = 0;
        additionalMinislotsChB = 0;
        if (vCycleCounter == gCycleCountMax) {
            vCycleCounter = 0;
            registerStaticSlots();
            registerDynamicSlots();
        } else {
            vCycleCounter++;
        }
        changeDrift();
        adjustMacrotick();
        emit(newCycle, 1L);
        cycles++;
        lastCycleStart = simTime();
        lastCycleTicks += getCycleTicks();
        correctEvents();
        scheduleAt(lastCycleStart + gdMacrotick * getCycleTicks(), msg);
        scheduleAt(lastCycleStart + (getCycleTicks() - gdNIT) * gdMacrotick,
                new SchedulerEvent("NIT", NIT_EVENT));
        newCyclemsg = msg;
        EV << "NEW CYCLE!! New Macrotick = " << gdMacrotick
                  << "!! Next cycle in "
                  << lastCycleStart + gdMacrotick * getCycleTicks() << "\n";
    } else if (msg->isSelfMessage() && msg->getKind() == NIT_EVENT) {
        FRSync *frSync = (FRSync*) (getParentModule()->getSubmodule("frSync"));
        if (vCycleCounter % 2 == 0) {
            frSync->offsetCorrectionCalculation(vCycleCounter);
        } else {
            zOffsetCorrection = frSync->offsetCorrectionCalculation(
                    vCycleCounter);
            zRateCorrection = frSync->rateCorrectionCalculation();
            correctNewCycle();
            frSync->resetTables();
        }
        EV << "Offset: " << zOffsetCorrection << " Rate: " << zRateCorrection
                  << "\n";
        delete msg;
    } else if (msg->isSelfMessage()
            && (msg->getKind() == STATIC_EVENT || DYNAMIC_EVENT)) {
        SchedulerEvent *event = (SchedulerEvent*) msg;
        registeredEvents.remove(event);
        sendDirect(event, event->getDestinationGate());
    }
}

void FRScheduler::registerStaticSlots() {
    SchedulerActionTimeEvent *event;
    std::list<unsigned int> staticSlotsChA;
    std::list<unsigned int> staticSlotsChB;
    int cycleNr;

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
        event = new SchedulerActionTimeEvent("Static Event", STATIC_EVENT);
        event->setDestinationGate(gateFRApp);
        event->setSyncFrameIndicator(false);
        if ((staticSlotsChA.front() == staticSlotsChB.front())
                && !staticSlotsChA.empty() && !staticSlotsChB.empty()) {
            cycleNr = ceil((staticSlotsChA.front() - 1) / gNumberOfStaticSlots);
            event->setChannel(2);
            event->setFrameID(
                    staticSlotsChA.front() - cycleNr * gNumberOfStaticSlots);
            staticSlotsChA.pop_front();
            staticSlotsChB.pop_front();
        } else if ((staticSlotsChA.front() < staticSlotsChB.front()
                && !staticSlotsChA.empty()) || staticSlotsChB.empty()) {
            cycleNr = ceil((staticSlotsChA.front() - 1) / gNumberOfStaticSlots);
            event->setChannel(0);
            event->setFrameID(
                    staticSlotsChA.front() - cycleNr * gNumberOfStaticSlots);
            staticSlotsChA.pop_front();
        } else {
            cycleNr = ceil((staticSlotsChB.front() - 1) / gNumberOfStaticSlots);
            event->setChannel(1);
            event->setFrameID(
                    staticSlotsChB.front() - cycleNr * gNumberOfStaticSlots);
            staticSlotsChB.pop_front();
        }
        event->setAction_time(
                getStaticSlotActionTime(event->getFrameID())
                        + cycleNr * getCycleTicks());
        if (event->getFrameID() == syncFrame) {
            event->setSyncFrameIndicator(true);
        }
        registerEvent(event);
    }
}

void FRScheduler::registerDynamicSlots() {
    const char *slots = par("dynamicSlotsChA");
    int slot;
    int cycleNr;
    cStringTokenizer tokenizerChA(slots);
    while (tokenizerChA.hasMoreTokens()) {
        slot = atoi(tokenizerChA.nextToken());
        cycleNr = ceil((slot - 1) / gNumberOfMinislots);
        SchedulerActionTimeEvent *event = new SchedulerActionTimeEvent(
                "Dynamic Event", DYNAMIC_EVENT);
        event->setFrameID(getDynamicSlot(slot - cycleNr * gNumberOfMinislots));
        event->setAction_time(
                getDynamicSlotActionTime(event->getFrameID())
                        + cycleNr * getCycleTicks());
        event->setChannel(0);
        event->setDestinationGate(gateFRApp);
        registerEvent(event);
    }

    slots = par("dynamicSlotsChB");
    cStringTokenizer tokenizerChB(slots);
    while (tokenizerChB.hasMoreTokens()) {
        slot = atoi(tokenizerChB.nextToken());
        cycleNr = ceil((slot - 1) / gNumberOfMinislots);
        SchedulerActionTimeEvent *event = new SchedulerActionTimeEvent(
                "Dynamic Event", DYNAMIC_EVENT);
        event->setFrameID(getDynamicSlot(slot - cycleNr * gNumberOfMinislots));
        event->setAction_time(
                getDynamicSlotActionTime(event->getFrameID())
                        + cycleNr * getCycleTicks());
        event->setChannel(1);
        event->setDestinationGate(gateFRApp);
        registerEvent(event);
    }
}

double FRScheduler::getMicroPerMacro() {
    return (double) (pMicroPerCycle + zRateCorrection)
            / (double) getCycleTicks();
}

void FRScheduler::adjustMacrotick() {
    gdMacrotick = getMicroPerMacro() * currentTick;
}

unsigned int FRScheduler::getTicks() {
    if (simTime() >= lastCycleStart) {
        return round(((simTime() - lastCycleStart) / gdMacrotick).dbl());
    } else {
        return cycleTicks
                - round(((lastCycleStart - simTime()) / gdMacrotick).dbl());
    }
}

unsigned long FRScheduler::getTotalTicks() {
    return lastCycleTicks + getTicks();
}

unsigned int FRScheduler::getCycles() {
    return cycles;
}

bool FRScheduler::registerEvent(SchedulerEvent *event) {
    take(event);
    registeredEvents.push_back(event);
    if (event->getKind() == STATIC_EVENT || DYNAMIC_EVENT) {
        SchedulerActionTimeEvent *actionTimeEvent =
                (SchedulerActionTimeEvent*) event;
        if (actionTimeEvent->getAction_time() > getTicks()) {
            scheduleAt(
                    lastCycleStart
                            + gdMacrotick * actionTimeEvent->getAction_time(),
                    actionTimeEvent);
        } else {
            scheduleAt(
                    lastCycleStart
                            + gdMacrotick
                                    * (actionTimeEvent->getAction_time()
                                            + getCycleTicks()),
                    actionTimeEvent);
        }
    }
    return true;
}

void FRScheduler::changeDrift() {
    double newDriftChange = uniform(-maxDriftChange, maxDriftChange);
    double newTick = currentTick + newDriftChange;
    if ((newTick - pdMicrotick) > maxDrift)
        currentTick = pdMicrotick + maxDrift;
    else if ((newTick - pdMicrotick) < -maxDrift)
        currentTick = pdMicrotick - maxDrift;
    else
        currentTick = newTick;
}

void FRScheduler::correctEvents() {
    for (std::list<SchedulerEvent*>::const_iterator registeredEvent =
            registeredEvents.begin(); registeredEvent != registeredEvents.end();
            registeredEvent++) {
        if ((*registeredEvent)->getKind() == DYNAMIC_EVENT || STATIC_EVENT) {
            SchedulerActionTimeEvent *actionTimeEvent =
                    (SchedulerActionTimeEvent*) *registeredEvent;
            cancelEvent(actionTimeEvent);
            if (actionTimeEvent->getAction_time() > getTicks()) {
                scheduleAt(
                        lastCycleStart
                                + gdMacrotick
                                        * (actionTimeEvent->getAction_time()
                                                - vCycleCounter
                                                        * getCycleTicks()),
                        actionTimeEvent);
            } else if (actionTimeEvent->getAction_time() == getTicks()) {
                scheduleAt(simTime(), actionTimeEvent);
            }
        } else if ((*registeredEvent)->getKind() == NIT_EVENT) {
            cancelEvent(*registeredEvent);
            scheduleAt(lastCycleStart + (getCycleTicks() - gdNIT) * gdMacrotick,
                    *registeredEvent);
        }

    }
}

void FRScheduler::correctNewCycle() {
    cancelEvent(newCyclemsg);
    scheduleAt(
            (lastCycleStart + gdMacrotick * getCycleTicks())
                    + zOffsetCorrection * currentTick, newCyclemsg);
}

void FRScheduler::setFRAppGate(cGate *gate) {
    gateFRApp = gate;
}

unsigned int FRScheduler::getStaticSlotActionTime(int frameID) {
    return ((frameID - 1) * gdStaticSlot) + gdActionPointOffset;
}

unsigned int FRScheduler::getDynamicSlotActionTime(int frameID) {
    return (((frameID - gNumberOfStaticSlots - 1) * gdMinislot)
            + (gNumberOfStaticSlots * gdStaticSlot))
            + gdMinislotActionPointOffset;
}

unsigned int FRScheduler::getCycleTicks() {
    return gdStaticSlot * gNumberOfStaticSlots + gdMinislot * gNumberOfMinislots
            + gdSymbolWindow + gdNIT;
}

unsigned int FRScheduler::getCycleCounter() {
    return vCycleCounter;
}

unsigned int FRScheduler::getSlotCounter() {
    Enter_Method_Silent();
    return ((simTime() - lastCycleStart) / (gdStaticSlot * gdMacrotick)).dbl()
            + 1; // works only for static segment
}

unsigned int FRScheduler::getDynamicSlot(int slot) {
    return slot + gNumberOfStaticSlots;
}

void FRScheduler::dynamicFrameReceived(int64 bitLength, unsigned int channel) {
    Enter_Method_Silent();
    int neededMinislots = ceil(
            ceil(
                    ((double) bitLength / ((double) busSpeed * 1024 * 1024))
                            / gdMacrotick) / gdMinislot);
    EV << "needed minislots: " << neededMinislots << "\n";
    if (channel == 0) {
        additionalMinislotsChA += neededMinislots - 1;
    } else {
        additionalMinislotsChB += neededMinislots - 1;
    }
    if (neededMinislots > 1) {
        std::list<SchedulerEvent*> toDelete;
        for (std::list<SchedulerEvent*>::const_iterator registeredEvent =
                registeredEvents.begin();
                registeredEvent != registeredEvents.end(); registeredEvent++) {
            if ((*registeredEvent)->getKind() == DYNAMIC_EVENT) {
                SchedulerActionTimeEvent *actionTimeEvent =
                        (SchedulerActionTimeEvent*) *registeredEvent;
                if ((actionTimeEvent->getAction_time()
                        - vCycleCounter * getCycleTicks()) > getTicks()
                        && actionTimeEvent->getChannel() == channel
                        && actionTimeEvent->getArrivalTime()
                                < lastCycleStart + getCycleTicks() * gdMacrotick) {
                    cancelEvent(actionTimeEvent);
                    if (channel == 0) {
                        actionTimeEvent->setAction_time(
                                getDynamicSlotActionTime(
                                        actionTimeEvent->getFrameID())
                                        + additionalMinislotsChA * gdMinislot);
                    } else {
                        actionTimeEvent->setAction_time(
                                getDynamicSlotActionTime(
                                        actionTimeEvent->getFrameID())
                                        + additionalMinislotsChB * gdMinislot);
                    }
                    EV << "getactiontime: " << actionTimeEvent->getAction_time()
                              << "\n";
                    if ((actionTimeEvent->getAction_time())
                            <= getCycleTicks() - gdSymbolWindow - gdNIT
                                    - gdMinislot + gdMinislotActionPointOffset
                                    + 1) {
                        //TODO FIXEN
                        SimTime sched = lastCycleStart + actionTimeEvent->getAction_time()* gdMacrotick;
                        if(sched>simTime()){
                            scheduleAt(
                                    lastCycleStart
                                            + actionTimeEvent->getAction_time()
                                                    * gdMacrotick, actionTimeEvent);
                        }
                        else{
                            scheduleAt(simTime(),actionTimeEvent);
                        }
                        //ENDE FIXEN

                    } else {
                        toDelete.push_back(*registeredEvent);
                        EV
                                  << "Dynamic frame deleted. Not enough remaining minislots.\n";
                    }
                }
            }
        }
        if (toDelete.size() > 0) {
            for (unsigned int i = 0; i < toDelete.size(); ++i) {
                registeredEvents.remove((SchedulerEvent*) toDelete.front());
                delete toDelete.front();
                toDelete.pop_front();
            }
        }
    }
}

int FRScheduler::calculateDeviationValue() {
    return ((simTime()
            - (lastCycleStart
                    + ((getSlotCounter() - 1) * gdStaticSlot
                            + gdActionPointOffset) * gdMacrotick)).dbl())
            / currentTick;
}