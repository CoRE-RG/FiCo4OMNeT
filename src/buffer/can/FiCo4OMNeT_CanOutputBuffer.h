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

#ifndef __FICO4OMNET_CANOUTPUTBUFFER_H_
#define __FICO4OMNET_CANOUTPUTBUFFER_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "FiCo4OMNeT_CanBuffer.h"
#include "FiCo4OMNeT_CanBusLogic.h"
#include "FiCo4OMNeT_CanTrafficSourceAppBase.h"
#include "FiCo4OMNeT_CanPortOutput.h"

namespace FiCo4OMNeT {

/**
 * @brief This buffer holds messages which will be sent to the bus.
 *
 * @ingroup Buffer
 *
 * @author Stefan Buschmann
 */
class CanOutputBuffer: public CanBuffer {

public:
    /**
     *
     */
    virtual ~CanOutputBuffer();
    /**
     * @brief Is called by the bus logic to permit the frame transmission.
     */
    virtual void receiveSendingPermission(int id);

    /**
     * @brief Is called when the frame transmission is completed.
     */
    virtual void sendingCompleted();

    /**
     * @brief Puts the frame into the collection and informs the connected gates about the receiption.
     *
     * @param frame The DataFrame to put in the buffer.
     *
     */
    virtual void putFrame(cMessage* msg);

protected:
    /**
     * @brief This method registers a frame at the bus for arbitration.
     */
    virtual void registerForArbitration(int id, bool rtr);

    /**
     * @brief Unregister from arbitration at the bus.
     */
    virtual void checkoutFromArbitration(int id);
};

}

#endif /* CANOUTPUTBUFFER_H_ */
