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

#ifndef FICO4OMNET_RESULTFILTERS_H
#define FICO4OMNET_RESULTFILTERS_H

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"

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
    virtual void addDelegate(cResultListener *delegate) override;
};


class RmNaNFilter : public cNumericResultFilter
{
    private:
        bool hadValues;
    public:
        RmNaNFilter();
        virtual bool process(simtime_t& t, double& value, cObject *details) override;
    protected:
        virtual void finish(cResultFilter * prev) override;
};


}

#endif
