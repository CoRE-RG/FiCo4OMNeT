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

}

#endif
