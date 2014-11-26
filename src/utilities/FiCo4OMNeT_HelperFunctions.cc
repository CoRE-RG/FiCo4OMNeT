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
#include "FiCo4OMNeT_HelperFunctions.h"


namespace FiCo4OMNeT {

cGate* gateByFullPath(const std::string &path)
{
    std::size_t pos = path.rfind('.');
    if (pos != std::string::npos)
    {
        std::string modulePath = path.substr(0, pos);
        std::string gateName = path.substr(pos + 1);
        cModule* module = cSimulation::getActiveSimulation()->getModuleByPath(modulePath.c_str());
        if (module)
        {
            return module->gate(gateName.c_str());
        }
    }
    return NULL;
}

//cGate* gateByShortPath(const std::string &nameAndGate, cModule *from)
//{
//    std::size_t pos = nameAndGate.rfind('.');
//    if (pos != std::string::npos)
//    {
//        std::string modulePath = nameAndGate.substr(0, pos);
//        std::string gateName = nameAndGate.substr(pos + 1);
//        cModule* module = findModuleWhereverInNode(modulePath.c_str(), from);
//        if (module)
//        {
//            return module->gate(gateName.c_str());
//        }
//    }
//    return NULL;
//}
}
