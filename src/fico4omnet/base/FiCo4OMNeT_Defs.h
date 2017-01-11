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

#ifndef FICO4OMNET_DEFS_H
#define FICO4OMNET_DEFS_H

//OMNeT++
#include <omnetpp.h>

#include "../features.h"

/**
 * Check for minimal OMNeT++ Version requirements
 */
#if OMNETPP_VERSION < 0x0500
#  error At least OMNeT++/OMNEST version 5.0 required
#endif

/**
 * Definition of FICO4OMNET version
 */
#define FICO4OMNET_VERSION 0x0100


#endif
