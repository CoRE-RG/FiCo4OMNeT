#ifndef __FICO4OMNET_RESULTFILTERS_H
#define __FICO4OMNET_RESULTFILTERS_H

//OMNeT++
#include "cresultfilter.h"

namespace FiCo4OMNeT {

/**
 * Filter that expects a cMessage and outputs its age in seconds
 * (t - msg->getCreationTime()).
 */
class TimestampAgeFilter: public cObjectResultFilter {
    using cObjectResultFilter::receiveSignal;
public:
    void receiveSignal(cResultFilter *prev, simtime_t_cref t,
            cObject *object);
};

/**
 * Filter that expects a CanDataFrame, ErrorFrame or FRFrame and outputs its ID
 */
class IDFilter: public cObjectResultFilter {
    using cObjectResultFilter::receiveSignal;
public:
    void receiveSignal(cResultFilter *prev, simtime_t_cref t,
            cObject *object);
};

/**
 * Filter that expects a CanDataFrame, ErrorFrame or FRFrame and outputs its ID
 */
class LowHighRatioFilter: public cNumericResultFilter {
private:
    simtime_t low;
    simtime_t high;
    double last;
    simtime_t last_time;
public:
    LowHighRatioFilter();
    virtual bool process(simtime_t& t, double& value);
};

class RemoveNaNFilter: public cNumericResultFilter {
public:
    virtual bool process(simtime_t& t, double& value);
    virtual void addDelegate(cResultListener *delegate);
};


class RmNaNFilter : public cNumericResultFilter
{
    private:
        bool hadValues;
    public:
        RmNaNFilter();
        virtual bool process(simtime_t& t, double& value);
    protected:
        virtual void finish(cResultFilter * prev);
};


}

#endif
