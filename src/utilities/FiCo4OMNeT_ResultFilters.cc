#include "FiCo4OMNeT_ResultFilters.h"

#include "cmessage.h"

namespace FiCo4OMNeT {

Register_ResultFilter("timestampAge", TimestampAgeFilter);

void TimestampAgeFilter::receiveSignal(cResultFilter *prev, simtime_t_cref t, cObject *object)
{
    if (dynamic_cast<cMessage *>(object))
    {
        cMessage *msg = (cMessage *)object;
        fire(this, t, t - msg->getTimestamp());
    }
}

}
