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

#ifndef FICO4OMNET_SCHEDULEREVENT_H_
#define FICO4OMNET_SCHEDULEREVENT_H_

#include "fico4omnet/scheduler/flexray/SchedulerMessage_m.h"

namespace FiCo4OMNeT {

using namespace omnetpp;

/**
 * @brief Redefines SchedulerEvent_Base and adds destinationGate
 */
class SchedulerEvent : public SchedulerEvent_Base
{
    private:
        cGate *destinationGate_var;

    public:
        SchedulerEvent(const char *name = nullptr, short kind = 0) :
            SchedulerEvent_Base(name, kind)
        {
            destinationGate_var = nullptr;
        }

        SchedulerEvent(const SchedulerEvent& other) :
            SchedulerEvent_Base(other.getName())
        {
            operator=(other);
        }

        SchedulerEvent& operator=(const SchedulerEvent& other)
        {
            SchedulerEvent_Base::operator=(other);
            return *this;
        }

        virtual SchedulerEvent *dup() const
        {
            return new SchedulerEvent(*this);
        }

        // ADD CODE HERE to redefine and implement pure virtual functions from SchedulerEvent_Base
        virtual cGate* getDestinationGate() const;
        virtual void setDestinationGate(cGate *destinationGate);
};

}
#endif
