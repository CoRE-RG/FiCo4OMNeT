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

#include "CanTrafficSourceAppBaseStoppable.h"

namespace FiCo4OMNeT {

Define_Module(CanTrafficSourceAppBaseStoppable);

void CanTrafficSourceAppBaseStoppable::initialize(int stage) {
    if (stage == 0) {
        handleParameterChange(nullptr);
    }
    CanTrafficSourceAppBase::initialize(stage);
}

void CanTrafficSourceAppBaseStoppable::frameTransmission(CanDataFrame *df) {
    if (!(simTime() >= this->_endTime)) {
        CanTrafficSourceAppBase::frameTransmission(df);
    } else if (!df->isSelfMessage()){
        delete df;
    }
}

void CanTrafficSourceAppBaseStoppable::handleParameterChange(const char* parname) {
    if (!parname || !strcmp(parname, "endTime")) {
        this->_endTime = par("endTime");
    }
}

} //namespace
