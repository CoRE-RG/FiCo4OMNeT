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

#ifndef __FICO4OMNET_BUSPORT_H_
#define __FICO4OMNET_BUSPORT_H_

#include <omnetpp.h>
//#include "CanBusLogic.h"

namespace FiCo4OMNeT {

/**
 * @brief Represents the connection between the bus and all connected nodes.
 * It forwards received frames to the bus logic and outgoing frames are forwarded to all connected nodes.
 *
 *
 * @ingroup Bus
 *
 * @author Stefan Buschmann
 */
class BusPort : public cSimpleModule {

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

    public:
        /**
         * @brief Forwards an Frame to all participants
         *
         * A copy of the Frame is generated and subsequently sent to all participants.
         * The original message will be deleted afterwards.
         *
         */
        virtual void forward_to_all(cMessage *msg);

        /**
         * @brief Sends a message to a single node.
         */
        virtual void sendMsgToNode(cMessage *msg, int id);
};

}

#endif /* BUSPORT_H_ */
