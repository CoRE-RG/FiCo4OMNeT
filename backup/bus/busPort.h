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

#ifndef BUSPORT_H_
#define BUSPORT_H_

#include <omnetpp.h>
#include "canBusApp.h"

using namespace std;

class busPort : public cSimpleModule {

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

        virtual void sendMsg(cMessage *msg, int id);
};

Define_Module(busPort);

#endif /* BUSPORT_H_ */
