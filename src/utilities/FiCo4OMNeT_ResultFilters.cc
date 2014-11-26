#include "FiCo4OMNeT_ResultFilters.h"

#include "cmessage.h"
#include "CanDataFrame_m.h"
#include "ErrorFrame_m.h"
#include "FRFrame_m.h"

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

Register_ResultFilter("ID", IDFilter);

void IDFilter::receiveSignal(cResultFilter *prev, simtime_t_cref t, cObject *object)
{
#ifdef WITH_CAN_COMMON
    if (CanDataFrame* dataFrame = dynamic_cast<CanDataFrame *>(object))
    {
        fire(this, t, (unsigned long) dataFrame->getCanID());
        return;
    }
    if (ErrorFrame* errorFrame = dynamic_cast<ErrorFrame *>(object))
    {
        fire(this, t, (unsigned long) errorFrame->getCanID());
        return;
    }
    if (ErrorFrame* errorFrame = dynamic_cast<ErrorFrame *>(object))
    {
        fire(this, t, (unsigned long) errorFrame->getCanID());
        return;
    }
#endif

#ifdef WITH_FR_COMMON
    if (FRFrame* frFrame = dynamic_cast<FRFrame *>(object))
    {
        fire(this, t, (unsigned long) frFrame->getFrameID());
        return;
    }
#endif
}

}
