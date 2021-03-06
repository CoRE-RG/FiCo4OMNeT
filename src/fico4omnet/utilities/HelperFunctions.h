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

#ifndef FICO4OMNET_HELPERFUNCTIONS_H_
#define FICO4OMNET_HELPERFUNCTIONS_H_

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"

namespace FiCo4OMNeT {

using namespace omnetpp;

/**
 * @brief Returns the gate defined by an object path.
 *
 * @param path the path to the gate
 * @return reference to the gate defined by path or NULL if no such gate was found
 *
 * @author Till Steinbach
 */
cGate* gateByFullPath(const std::string &path);

/**
 * @brief Returns the gate of a node defined by: modulename.gatename.
 *
 * @param nameAndGate name of the module and gatename in the format: modulename.gatename
 * @param from a module of the node in which the gate can be found
 * @return reference to the gate defined by path or NULL if no such gate was found
 *
 * @author Till Steinbach
 */
cGate* gateByShortPath(const std::string &nameAndGate, cModule *from);

cModule *findModuleWhereverInNode(const char *name, cModule *from);

inline bool _isNetworkNode(cModule *mod);


}

#endif /* HELPERFUNCTIONS_H_ */
