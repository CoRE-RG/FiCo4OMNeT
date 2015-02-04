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

#include "FiCo4OMNeT_FRScheduler.h"

namespace FiCo4OMNeT {

Define_Module(FRScheduler);

//simsignal_t FRScheduler::newCycle = SIMSIGNAL_NULL;

void FRScheduler::initialize() {
    gCycleCountMax = getParentModule()->par("gCycleCountMax");
    vCycleCounter = gCycleCountMax;
    maxDriftChange = getParentModule()->par("maxDriftChange").doubleValue(); //[]
    maxDrift = getParentModule()->par("maxDrift").doubleValue(); //[]
    pdMicrotick = getParentModule()->par("pdMicrotick").doubleValue(); //[ns]
    gdMacrotick = getParentModule()->par("gdMacrotick").doubleValue(); //[�s]
    gdStaticSlot = getParentModule()->par("gdStaticSlot"); //[MT]
    gdMinislot = getParentModule()->par("gdMinislot"); //[MT]
    gdNIT = getParentModule()->par("gdNIT"); //[MT]
    gdSymbolWindow = getParentModule()->par("gdSymbolWindow"); //[MT]
    gNumberOfMinislots = getParentModule()->par("gNumberOfMinislots");
    gNumberOfStaticSlots = getParentModule()->par("gNumberOfStaticSlots");
    gdActionPointOffset = getParentModule()->par("gdActionPointOffset"); //[MT]
    gdMinislotActionPointOffset = getParentModule()->par(
            "gdMinislotActionPointOffset"); //[MT]
    bandwidth = getParentModule()->par("bandwidth").doubleValue();
//    syncFrame = getParentModule()->par("syncFrame");

    currentTick = pdMicrotick;
    newCycle = SIMSIGNAL_NULL;
    newCycle = registerSignal("newCycle");
    scheduleAt(simTime(), new SchedulerEvent("NEW_CYCLE", NEW_CYCLE));
    lastCycleStart = simTime();
    pMicroPerCycle = static_cast<unsigned int>((getCycleTicks() * gdMacrotick) / pdMicrotick);

    zRateCorrection = 0;
    zOffsetCorrection = 0;

//    FRApp *frApp = (FRApp*) (getParentModule()->getSubmodule("frApp"));
//    frApp->setMaxRandom(
//            (bandwidth * 1024 * 1024
//                    * (gNumberOfMinislots * gdMinislot * gdMacrotick)) / 4);
}

void FRScheduler::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage() && msg->getKind() == NEW_CYCLE) {
        additionalMinislotsChA = 0;
        additionalMinislotsChB = 0;
        if (vCycleCounter == gCycleCountMax) {
            vCycleCounter = 0;
//            registerStaticSlots();
//            registerDynamicSlots();
        } else {
            vCycleCounter++;
        }
        changeDrift();
        adjustMacrotick();
        emit(newCycle, static_cast<long> (vCycleCounter));
        cycles++;
        lastCycleStart = simTime();
        lastCycleTicks += getCycleTicks();
        correctEvents();
        scheduleAt(lastCycleStart + gdMacrotick * getCycleTicks(), msg);
        scheduleAt(lastCycleStart + (getCycleTicks() - gdNIT) * gdMacrotick,
                new SchedulerEvent("NIT", NIT_EVENT));
        newCyclemsg = msg;
        EV << vCycleCounter << " NEW CYCLE!! New Macrotick = " << gdMacrotick
                  << "!! Next cycle in "
                  << lastCycleStart + gdMacrotick * getCycleTicks() << "\n";
    } else if (msg->isSelfMessage() && msg->getKind() == NIT_EVENT) {
        FRSync *frSync = dynamic_cast<FRSync*> (getParentModule()->getSubmodule("frSync"));
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
        SchedulerEvent *event = dynamic_cast<SchedulerEvent*> (msg);
        registeredEvents.remove(event);
        sendDirect(event, event->getDestinationGate());
    }
}

double FRScheduler::getMicroPerMacro() {
    return static_cast<double> (static_cast<int> (pMicroPerCycle) + zRateCorrection)
            / static_cast<double> (getCycleTicks());
}

void FRScheduler::adjustMacrotick() {
    gdMacrotick = getMicroPerMacro() * currentTick;
}

unsigned int FRScheduler::getTicks() {
    if (simTime() >= lastCycleStart) {
        return static_cast<unsigned int> (round(((simTime() - lastCycleStart) / gdMacrotick).dbl()));
    } else {
        return static_cast<unsigned int> (cycleTicks
                - round(((lastCycleStart - simTime()) / gdMacrotick).dbl()));
    }
}

unsigned long FRScheduler::getTotalTicks() {
    return lastCycleTicks + getTicks();
}

unsigned long FRScheduler::getCycles() {
    return cycles;
}

bool FRScheduler::registerEvent(SchedulerEvent *event) {
    take(event);
    registeredEvents.push_back(event);
    if (event->getKind() == STATIC_EVENT || DYNAMIC_EVENT) {
        SchedulerActionTimeEvent *actionTimeEvent =
                dynamic_cast<SchedulerActionTimeEvent*> (event);
        if (event->getKind() == DYNAMIC_EVENT) {
            actionTimeEvent->setAction_time(
                    getDynamicSlotActionTime(actionTimeEvent->getFrameID()));

        } else {
            actionTimeEvent->setAction_time(
                    getStaticSlotActionTime(actionTimeEvent->getFrameID()));
        }

        if (vCycleCounter <= actionTimeEvent->getCycleNr()) {
            actionTimeEvent->setAction_time(actionTimeEvent->getAction_time() + (actionTimeEvent->getCycleNr() - vCycleCounter) * getCycleTicks());
        } else {
            actionTimeEvent->setAction_time(actionTimeEvent->getAction_time() + (gCycleCountMax - vCycleCounter + actionTimeEvent->getCycleNr()) * getCycleTicks());
        }

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
                    dynamic_cast<SchedulerActionTimeEvent*> (*registeredEvent);
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

void FRScheduler::setFRAppGate(cGate *appGate) {
    gateFRApp = appGate;
}

unsigned int FRScheduler::getStaticSlotActionTime(unsigned int frameID) {
    unsigned int ret;
    if (frameID > 0) {
        ret = ((frameID - 1) * gdStaticSlot) + gdActionPointOffset;
    } else {
        ret = (frameID * gdStaticSlot) + gdActionPointOffset;
    }
    return ret;
}

unsigned int FRScheduler::getDynamicSlotActionTime(unsigned int frameID) {
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
    Enter_Method_Silent
    ();
    return (unsigned int)((simTime() - lastCycleStart) / (gdStaticSlot * gdMacrotick)).dbl()
            + 1; // works only for static segment
}

unsigned int FRScheduler::getDynamicSlot(int slot) {
    return slot + gNumberOfStaticSlots;
}

void FRScheduler::dynamicFrameReceived(int64 bitLength, unsigned int channel) {
    Enter_Method_Silent
    ();
//    int neededMinislots = ceil(
//                ceil(
//                        ((double) bitLength / ((double) bandwidth * 1024 * 1024))
//                                / gdMacrotick) / gdMinislot);
    int neededMinislots = (int) (ceil(
                ceil(
                        ((double) bitLength / bandwidth)
                                / gdMacrotick) / gdMinislot));
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
                                < lastCycleStart
                                        + getCycleTicks() * gdMacrotick) {
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
                        SimTime sched = lastCycleStart
                                + actionTimeEvent->getAction_time()
                                        * gdMacrotick;
                        if (sched > simTime()) {
                            scheduleAt(
                                    lastCycleStart
                                            + actionTimeEvent->getAction_time()
                                                    * gdMacrotick,
                                    actionTimeEvent);
                        } else {
                            scheduleAt(simTime(), actionTimeEvent);
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
    return (int)(((simTime()
            - (lastCycleStart
                    + ((getSlotCounter() - 1) * gdStaticSlot
                            + gdActionPointOffset) * gdMacrotick)).dbl())
            / currentTick);
}

}
