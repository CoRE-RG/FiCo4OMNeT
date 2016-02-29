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

#ifndef __FICO4OMNET_FRSCHEDULER_H_
#define __FICO4OMNET_FRSCHEDULER_H_

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"
#include "fico4omnet/scheduler/flexray/SchedulerEvent.h"

namespace FiCo4OMNeT {

// TODO Documentation
class FRScheduler : public cSimpleModule {
	private:
        /**
         * @brief Simulation time when the last cycle started
         */
        SimTime lastCycleStart;

        /**
         * @brief Number of ticks since the simulation started
         * when the last cycle started
         */
        unsigned long lastCycleTicks;

        /**
         * @brief List of registered SchedulerEvents.
         *
         * Used to reregister after clock tick changes.
         */
        std::list<SchedulerEvent*> registeredEvents;

        cGate *gateFRApp;

        /**
         *
         */
        simsignal_t newCycle;

        /**
         * @brief caches new cycle message
         */
        cMessage* newCyclemsg;

        /**
         * @brief caches max_drift_change parameter
         */
        double maxDriftChange;

        /**
		 * @brief caches max_drift parameter
		 */
		double maxDrift;
        /**
         * @brief caches current_tick parameter
         */
        double currentTick;
        /**
         * @brief caches cycle_ticks parameter
         */
        unsigned long cycleTicks;

        /**
         * @brief Number of cycles since the simulation started
         */
        unsigned long cycles;

        /**
         * @brief caches max_cycle_count parameter
         */
        unsigned int gCycleCountMax;

        /**
         * @brief counter for the current cycle
         */
        unsigned int vCycleCounter;

        /**
         * @brief caches duration_microtick parameter
         */
        double pdMicrotick; //[ns]

        /**
         * @brief caches duration_macrotick parameter
         */
        double gdMacrotick; //[]

        /**
         * @brief caches duration_static_slot parameter
         */
        unsigned int gdStaticSlot; //[MT]

        /**
         * @brief caches duration_minislot parameter
         */
        unsigned int gdMinislot; //[MT]

        /**
         * @brief caches duration_nit parameter
         */
        unsigned int gdNIT; //[MT]

        /**
         * @brief caches duration_symbol_window parameter
         */
        unsigned int gdSymbolWindow; //[MT]

        /**
         * @brief caches number_of_minislots parameter
         */
        unsigned int gNumberOfMinislots;

        /**
         * @brief caches number_of_static_slots parameter
         */
        unsigned int gNumberOfStaticSlots;

        /**
         * @brief caches actionpoint_offset parameter
         */
        unsigned int gdActionPointOffset; //[MT]

        /**
         * @brief caches minislot_actionpoint_offset parameter
         */
        unsigned int gdMinislotActionPointOffset; //[MT]

        /**
         * @brief caches bus_speed parameter
         */
        double bandwidth; //[MBit/s]

        /**
         * @brief number of microticks per cycle
         */
        unsigned int pMicroPerCycle; //[uT]

        /**
         * @brief last value of the offset correction
         */
        int zOffsetCorrection; //[uT]

        /**
         * @brief last value of the rate correction
         */
        int zRateCorrection; //[uT]

        int additionalMinislotsChA;
        int additionalMinislotsChB;

	protected:
//        static simsignal_t newCycle;

        /**
		 * Signal that is emitted every time the drift (Difference of configured and actual tick length) changes
		 */
		static simsignal_t currentDrift;

        /**
         * @brief Initialization of the module.
         */
        virtual void initialize();

        /**
         * @brief Handles all scheduler messages
         *
         * Registers self-messages for events sends messages to trigger events in
         * remote modules.
         *
         * @param msg the incoming message.
         */
        virtual void handleMessage(cMessage *msg);

//        /**
//         * @brief Registers all dynamic slots for this node for the next gCycleCountMax cycles.
//         */
//        virtual void registerDynamicSlots();
//
//        /**
//         * @brief Registers all static slots for this node for the next gCycleCountMax cycles.
//         */
//        virtual void registerStaticSlots();

        /**
         * @brief Returns the number of microticks per macrotick
         */
        virtual double getMicroPerMacro();

        /**
         * @brief Adjusts the duration of the macrotick.
         */
        virtual void adjustMacrotick();

    public:
        /**
         * @brief Constructor
         */
        FRScheduler();

        /**
         * @brief Returns the current number of ticks
         *
         * @return Number of ticks since last cycle start
         */
        virtual unsigned long getTicks();

        /**
         * @brief Returns the absolute number of ticks
         *
         * @return Number of ticks since simulation start
         */
        virtual unsigned long getTotalTicks();

        /**
         * @brief Returns the current number of cycles
         *
         * @return Number of cycles since simulation start
         */
        virtual unsigned long getCycles();

        /**
         * Register a new event in the scheduler. May fail if ActionTimeEvent is out of schedule
         *
         * @param event Pointer to the Event to be scheduled.
         * The scheduler will send the event according to the event type
         * @return returns true on success, else false
         *
         * @sa SchedulerEvent_Base, SchedulerEvent, SchedulerActionTimeEvent,
         * SchedulerTimerEvent
         */
        virtual bool registerEvent(SchedulerEvent *event);

        /**
		 * @brief Helper function to change the drift
		 *
		 * Can be overwritten to implement special clock drift behavior.
        */
        virtual void changeDrift();

        /**
		 * @brief Helper function to correct Events after tick length changes
		 */
        virtual void correctEvents();

        /**
         * @brief Adjusts the NEW_CYCLE event with the value of the offset correction.
         */
        virtual void correctNewCycle();

        /**
         * @brief Sets the schedulerIn gate of the FRApp module.
         */
        virtual void setFRAppGate(cGate *gate);

        /**
         * @brief Returns the number of macroticks for the given static slot in a cycle.
         */
        virtual unsigned int getStaticSlotActionTime(unsigned int slot);

        /**
         * @brief Returns the number of macroticks for the given dynamic slot in a cycle.
         */
        virtual unsigned int getDynamicSlotActionTime(unsigned int slot);

        /**
         * @brief Returns the number of macroticks in a cycle.
         */
        virtual unsigned int getCycleTicks();

        /**
         * @brief Returns the current cyclenumber.
         */
        virtual unsigned int getCycleCounter();

        /**
         * @brief Returns the current static slot.
         */
        virtual unsigned int getSlotCounter();

        /**
         * @brief Returns the slotnumber for the given dynamic slot.
         */
		virtual unsigned int getDynamicSlot(unsigned int slot);

        /**
         * @brief Adjusts the dynamic events in the current cycle when a dynamic frame is received.
         */
        virtual void dynamicFrameReceived(int64_t bitLength, unsigned int channel);

        /**
         * @brief Returns the deviation to the actionpoint in the current slot.
         */
        virtual int calculateDeviationValue();
};

}

#endif /* FRSCHEDULER_H_ */
