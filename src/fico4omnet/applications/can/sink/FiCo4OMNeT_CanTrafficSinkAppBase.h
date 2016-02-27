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

#ifndef __FICO4OMNET_CANTRAFFICSINKAPPBASE_H_
#define __FICO4OMNET_CANTRAFFICSINKAPPBASE_H_

//std
#include <string.h>
//OMNeT++
#include <omnetpp.h>
//FiCo4OMNeT
#include "FiCo4OMNeT_CanInputBuffer.h"
#include "FiCo4OMNeT_CanPortInput.h"
//Auto-generated messages
#include "CanDataFrame_m.h"

namespace FiCo4OMNeT {

/**
 * @brief Traffic sink application used to handle incomming messages.
 *
 * @ingroup Applications
 *
 * @author Stefan Buschmann
 */
class CanTrafficSinkAppBase: public cSimpleModule {

protected:
    /**
     * @brief Initialization of the module.
     */
    virtual void initialize();

    /**
     * @brief Collects incoming message and writes statistics.
     *
     * @param msg incoming frame
     */
    virtual void handleMessage(cMessage *msg);

    /**
     * @brief Requests a frame from the buffer.
     */
    void requestFrame();

    /**
     * @brief Number of messages currently in the Buffer
     */
    int bufferMessageCounter;

    /**
     * @brief Shows whether the application is working or not.
     */
    bool idle;

    /**
     * @brief The CAN-ID of the message which is currently processed.
     */
    unsigned int currentFrameID;

private:
    /**
     * @brief The sink processes the frame.
     *
     * @param workTime represents the time it takes to process the current frame.
     */
    void startWorkOnFrame(float workTime);

    /**
     * @brief Simsignal for received data frames.
     */
    simsignal_t rxDFSignal;

    /**
     * @brief Simsignal for received remote frames.
     */
    simsignal_t rxRFSignal;

    /**
     * @brief Simsignal for received data frames.
     */
    simsignal_t rxDFPayloadSignal;

    /**
     * @brief Simsignal for received remote frames.
     */
    simsignal_t rxRFPayloadSignal;

};

}

#endif /* CANTRAFFICSINKAPP_H_ */
