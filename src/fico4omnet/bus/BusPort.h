//Copyright (c) 2014, CoRE Research Group, Hamburg University of Applied Sciences
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification,
//are permitted provided that the following conditions are met:
//
//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
//3. Neither the name of the copyright holder nor the names of its contributors
//   may be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
//ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef FICO4OMNET_BUSPORT_H_
#define FICO4OMNET_BUSPORT_H_

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"

namespace FiCo4OMNeT {

using namespace omnetpp;

/**
 * @brief Represents the connection between the bus and all connected nodes.
 *
 * It forwards received frames to the bus logic and outgoing frames are forwarded to all connected nodes.
 *
 * @ingroup Bus
 *
 * @author Stefan Buschmann
 */
class BusPort : public cSimpleModule {

    protected:
        /**
         * @brief Messages from bus participants are forwarded to the inner module and messages from the inner module are forwarded to all bus participants.
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
         */
        virtual void forward_to_all(cMessage *msg);

        /**
         * @brief Sends a message to a single node.
         *
         * @param msg message to be sent
         * @param gateId identifier of the gate
         */
        virtual void sendMsgToNode(cMessage *msg, int gateId);
};

}

#endif /* BUSPORT_H_ */
