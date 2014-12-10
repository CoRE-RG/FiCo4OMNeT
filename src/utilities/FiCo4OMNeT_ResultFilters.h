#ifndef __FICO4OMNET_RESULTFILTERS_H
#define __FICO4OMNET_RESULTFILTERS_H

//OMNeT++
#include "cresultfilter.h"

namespace FiCo4OMNeT{

/**
 * Filter that expects a cMessage and outputs its age in seconds
 * (t - msg->getCreationTime()).
 */
class TimestampAgeFilter : public cObjectResultFilter
{
    public:
        virtual void receiveSignal(cResultFilter *prev, simtime_t_cref t, cObject *object);
};

/**
 * Filter that expects a CanDataFrame, ErrorFrame or FRFrame and outputs its ID
 */
class IDFilter : public cObjectResultFilter
{
    public:
        virtual void receiveSignal(cResultFilter *prev, simtime_t_cref t, cObject *object);
};

/**
 * Filter that expects a CanDataFrame, ErrorFrame or FRFrame and outputs its ID
 */
class LowHighRatioFilter : public cNumericResultFilter
{
    private:
        simtime_t low;
        simtime_t high;
        double last;
        simtime_t last_time;
    public:
        LowHighRatioFilter();
        virtual bool process(simtime_t& t, double& value);
};

}

#endif
