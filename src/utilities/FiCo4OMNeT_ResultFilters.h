#ifndef __FICO4OMNET_RESULTFILTERS_H
#define __FICO4OMNET_RESULTFILTERS_H

//OMNeT++
#include "omnetpp.h"

namespace FiCo4OMNeT {

using namespace omnetpp;

/**
 * Filter that expects a cMessage and outputs its age in seconds
 * (t - msg->getCreationTime()).
 */
class TimestampAgeFilter: public cObjectResultFilter {
    using cObjectResultFilter::receiveSignal;
public:
    void receiveSignal(cResultFilter *prev, simtime_t_cref t,
            cObject *object, cObject *details) override;
};

/**
 * Filter that expects a CanDataFrame, ErrorFrame or FRFrame and outputs its ID
 */
class IDFilter: public cObjectResultFilter {
    using cObjectResultFilter::receiveSignal;
public:
    void receiveSignal(cResultFilter *prev, simtime_t_cref t,
            cObject *object, cObject *details) override;
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
    virtual bool process(simtime_t& t, double& value, cObject *details) override;
};

class RemoveNaNFilter: public cNumericResultFilter {
public:
    virtual bool process(simtime_t& t, double& value, cObject *details) override;
    virtual void addDelegate(cResultListener *delegate);
};


class RmNaNFilter : public cNumericResultFilter
{
    private:
        bool hadValues;
    public:
        RmNaNFilter();
        virtual bool process(simtime_t& t, double& value, cObject *details) override;
    protected:
        virtual void finish(cResultFilter * prev);
};


}

#endif
