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

#ifndef __FICO4OMNET_CANPORTINPUT_H_
#define __FICO4OMNET_CANPORTINPUT_H_

#include <omnetpp.h>
#include <string.h>
#include "CanDataFrame_m.h"
#include "ErrorFrame_m.h"
#include "FiCo4OMNeT_CanPortOutput.h"

namespace FiCo4OMNeT {

/**
 * @brief Received messages are initially handled in this module.
 *
 * @ingroup Port
 *
 * @author Stefan Buschmann
 */
class CanPortInput: public cSimpleModule {
public:
    /**
     * @brief Source Applications can register their CanIDs so that incoming remote frames are forwarded to them.
     */
    virtual void registerOutgoingDataFrame(int canID, cGate* gate);

    /**
     * @brief Source Applications can register their remote frame CanIDs.
     */
    virtual void registerOutgoingRemoteFrame(int canID);

    /**
     * @brief Sink Applications can register the CanIDs which they are interested in.
     */
    virtual void registerIncomingDataFrame(int canID);

protected:
    /**
     * @brief Initialization of several variables.
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
     * @brief Simsignal for received data frames.
     */
    simsignal_t rcvdDFSignal;

    /**
     * @brief Simsignal for received remote frames.
     */
    simsignal_t rcvdRFSignal;

    /**
     * @brief Number of useful data frames received by this node.
     */
    int dataFramesReceived;

    /**
     * @brief Vector with IDs of useful incoming data frames
     */
    std::vector<int> incomingDataFrameIDs;

    /**
     * @brief Map with IDs and corresponding sourceApp-gates of useful incoming remote frames
     */
    std::map<int,cGate*> outgoingDataFrameIDs;

    /**
     * @brief Vector with IDs of outgoing remote frames
     */
    std::vector<int> outgoingRemoteFrameIDs;

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
     * @brief Currently scheduled Data Frame
     */
    CanDataFrame *scheduledDataFrame;

    /**
     * @brief Currently scheduled Error Frame
     */
    ErrorFrame *scheduledErrorFrame;

    /**
     * @brief Incoming Data frame is scheduled until receiving is completed.
     */
    virtual void receiveMessage(CanDataFrame *msg);

    /**
     * @brief This method generates an error message for the received data frame.
     */
    virtual void generateReceiveError(CanDataFrame *df);

    /**
     * @brief Checks whether the received frame is relevant for this node.
     */
    virtual bool checkExistence(CanDataFrame *df);

    /**
     * @brief Checks whether the frame with the corresponding ID is sent by this node.
     */
    virtual bool checkOutgoingDataFrames(int id);

    /**
     * @brief Checks whether the frame with the corresponding ID is sent by this node.
     */
    virtual bool checkOutgoingRemoteFrames(int id);

    /**
     * @brief Checks whether the frame is listed in the relevant incoming frames.
     */
    virtual bool checkIncomingDataFrames(int id);

    /**
     * @brief Calculates when the frame is ready to be forwarded based on the number of bits.
     */
    virtual double calculateScheduleTiming(int length);

    /**
     * @brief Sends the message to the output gate if incomingIDs contains the message ID or to the SourceApp in case of a remote frame.
     */
    virtual void forwardDataFrame(CanDataFrame *msg);

    /**
     * @brief Sends the error frame to the output gate
     */
    virtual void forwardOwnErrorFrame(ErrorFrame *ef);

    /**
     * @brief Called when a error frame is received.
     */
    virtual void handleExternErrorFrame(ErrorFrame *ef);
};

}

#endif /* CANPORTINPUT_H_ */
