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

#ifndef FICO4OMNET_FRTOPOLOGYPORT_H_
#define FICO4OMNET_FRTOPOLOGYPORT_H_

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"


namespace FiCo4OMNeT {

// TODO Documentation
class FRTopologyPort : public cSimpleModule {

	protected:
        /**
         * @brief Handles all scheduler messages
         *
         * Registers self-messages for events sends messages to trigger events in
         * remote modules.
         *
         * @param msg the incoming message.
         */
        virtual void handleMessage(cMessage *msg);
};


}

#endif /* FRTOPOLOGYPORT_H_ */
