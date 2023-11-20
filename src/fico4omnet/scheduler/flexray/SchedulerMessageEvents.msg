cplusplus {{
        #include "fico4omnet/scheduler/flexray/SchedulerEvent.h"
}}

import SchedulerMessage;

namespace FiCo4OMNeT;


//
// Message used for events of TTEScheduler that are triggered at a specified time in cycle (action_time).
// Should be created with message kind SchedulerMessageKind = ACTION_TIME_EVENT
//
// @see SchedulerMessageKind, SchedulerEvent
//
// @author Till Steinbach
message SchedulerActionTimeEvent extends SchedulerEvent {
        // Action time in cycle for the event in number of ticks
        unsigned int action_time;
        unsigned int frameID;
        unsigned int cycleNr;
        unsigned int channel;
        bool syncFrameIndicator;
}

//
// Message used for events of TTEScheduler that are triggered after a specified time (timer).
// Should be created with message kind SchedulerMessageKind = TIMER_EVENT
//
// @see SchedulerMessageKind, SchedulerEvent
//
// @author Till Steinbach
message SchedulerTimerEvent extends SchedulerEvent {
        // Time after that the event is triggered in number of ticks
        unsigned int timer;
}
