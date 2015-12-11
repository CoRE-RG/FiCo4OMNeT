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

#ifndef __FICO4OMNET_CANPORTOUTPUT_H_
#define __FICO4OMNET_CANPORTOUTPUT_H_

//OMNeT++
#include <omnetpp.h>
//FiCo4OMNeT auto-generated messages
#include "ErrorFrame_m.h"
#include "CanDataFrame_m.h"

namespace FiCo4OMNeT {

using namespace omnetpp;

/**
 * @brief Outgoing messages are handled in this module.
 *
 * @ingroup Port
 *
 * @author Stefan Buschmann
 */
class CanPortOutput: public cSimpleModule {

public:
    /**
     * @brief Constructor
     */
    CanPortOutput();
    
    /**
     * @brief Destructor
     */
    virtual ~CanPortOutput();

    /**
     * @brief Handles received error frames so that scheduled error events can be canceled.
     */
    virtual void handleReceivedErrorFrame();

    /**
     * @brief Is called when the transmission of a frame is completed.
     */
    virtual void sendingCompleted();

protected:
    /**
     * @brief Initialization of the module.
     */
    virtual void initialize();

    /**
     * @brief Handles all received messages
     *
     * @param msg the incoming message.
     */
    virtual void handleMessage(cMessage *msg);

private:
    /**
     * @brief Maximum size of an error frame.
     */
    static const int MAXERRORFRAMESIZE = 12;

    /**
     * @brief Simsignal for sent data frames.
     */
    simsignal_t sentDFSignal;

    /**
     * @brief Simsignal for sent remote frames.
     */
    simsignal_t sentRFSignal;

    /**
     * @brief Simsignal for receive errors.
     */
    simsignal_t receiveErrorsSignal;

    /**
     * @brief Simsignal for send errors.
     */
    simsignal_t sendErrorsSignal;

    /**
     * @brief Bandwidth of the bus in Mbps.
     */
    double bandwidth;

    /**
     * @brief Probability that an error for the received frame will occur.
     *
     */
    int errorperc;

    /**
     * @brief The currently scheduled error frame.
     */
    ErrorFrame *scheduledErrorFrame;

    /**
     * @brief True if an error frame for the current data frame is already received.
     */
    bool errorReceived;

    /**
     * @brief Initializes the values needed for the stats collection.
     */
    virtual void initializeStatisticValues();

    /**
     * @brief Calculates when the frame is ready to be forwarded based on the number of bits.
     *
     * @param length length of the frame in bit
     *
     * @return the duration in seconds until frame transmission is completed
     */
    virtual double calculateScheduleTiming(int length);

    /**
     * @brief Colors the connection to the bus to represent it is busy.
     */
    virtual void colorBusy();

    /**
     * @brief Colors the connection to the bus to represent it is idle.
     */
    virtual void colorIdle();

    /**
     * @brief Colors the connections to the bus to represent a transmission of an error frame.
     */
    virtual void colorError();
};

}

#endif /* CANPORTOUTPUT_H_ */
