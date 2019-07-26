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

#ifndef __FICO4OMNET_CANTRAFFICSOURCEAPPBASESTOPPABLE_H_
#define __FICO4OMNET_CANTRAFFICSOURCEAPPBASESTOPPABLE_H_

#include <omnetpp.h>
#include <string>
#include "CanTrafficSourceAppBase.h"

using namespace omnetpp;

namespace FiCo4OMNeT {

/**
 * TODO - Generated class
 */
class CanTrafficSourceAppBaseStoppable : public virtual CanTrafficSourceAppBase
{
  protected:
    virtual void frameTransmission(CanDataFrame *df) override;
    virtual void initialize(int stage) override;
    virtual void handleParameterChange(const char* parname) override;

  private:
    double _endTime;

};

} //namespace

#endif
