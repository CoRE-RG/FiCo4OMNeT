#include "fico4omnet/utilities/ResultFilters.h"

//Auto-generated messages
#include "fico4omnet/linklayer/can/messages/CanDataFrame_m.h"
#include "fico4omnet/linklayer/can/messages/ErrorFrame_m.h"
#include "fico4omnet/linklayer/flexray/messages/FRFrame_m.h"

namespace FiCo4OMNeT {

Register_ResultFilter("timestampAge", TimestampAgeFilter);

void TimestampAgeFilter::receiveSignal(cResultFilter *prev, simtime_t_cref t,
        cObject *object) {
    (void)prev;
    if (cMessage *msg = dynamic_cast<cMessage *>(object)) {
//        cMessage *msg = dynamic_cast<cMessage *> (object);
        fire(this, t, t - msg->getTimestamp());
    }
    else
    {
        fire(this, t, object);
    }
}

Register_ResultFilter("ID", IDFilter);

void IDFilter::receiveSignal(cResultFilter *prev, simtime_t_cref t,
        cObject *object) {
    (void)prev;
#ifdef WITH_CAN_COMMON
    if (CanDataFrame* dataFrame = dynamic_cast<CanDataFrame *>(object)) {
        fire(this, t, static_cast<unsigned long> (dataFrame->getCanID()));
        return;
    }
    if (ErrorFrame* errorFrame = dynamic_cast<ErrorFrame *>(object)) {
        fire(this, t, static_cast<unsigned long> (errorFrame->getCanID()));
        return;
    }
    if (ErrorFrame* errorFrame = dynamic_cast<ErrorFrame *>(object)) {
        fire(this, t, static_cast<unsigned long> (errorFrame->getCanID()));
        return;
    }
#endif

#ifdef WITH_FR_COMMON
    if (FRFrame* frFrame = dynamic_cast<FRFrame *>(object)) {
        fire(this, t, static_cast<unsigned long> (frFrame->getFrameID()));
        return;
    }
#endif
    fire(this, t, object);
}

Register_ResultFilter("lowHighRatio", LowHighRatioFilter);

LowHighRatioFilter::LowHighRatioFilter() {
    low = 0;
    high = 0;
    last = 0;
    last_time = 0;
}

bool LowHighRatioFilter::process(simtime_t& t, double& value) {
    if (this->last > 0)
        high += (t - this->last_time);
    else
        low += (t - this->last_time);
    this->last = value;
    this->last_time = t;
    if ((high + low) > 0)
        value = high / (high + low);
    else
        value = 0;
    return true;
}

Register_ResultFilter("rmNaN", RmNaNFilter);

RmNaNFilter::RmNaNFilter()
{
    this->hadValues = false;
}

bool RmNaNFilter::process(simtime_t& t, double& value)
{
    (void)t;
    (void)value;
    if(!this->hadValues){
        this->hadValues = true;
    }
    return true;
}

void RmNaNFilter::finish(cResultFilter * prev)
{
    (void)prev;
    if(!this->hadValues){
        fire(this, simTime(), static_cast<unsigned long> (0) );
    }
}

}
