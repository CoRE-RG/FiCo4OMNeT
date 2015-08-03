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

//std
#include <string.h>
//OMNeT++
#include <omnetpp.h>
//FiCo4OMNeT
#include "FiCo4OMNeT_CanPortOutput.h"
#include "FiCo4OMNeT_CanOutputBuffer.h"
//Auto-generated messages
#include "CanDataFrame_m.h"
#include "ErrorFrame_m.h"

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
     * @brief Modules can register their CanIDs so that incoming remote frames are forwarded to them.
     *
     * @param canID the can ID the module wants to register
     * @param gate the direct input port of the application
     */
    virtual void registerOutgoingDataFrame(unsigned int canID, cGate* gate);

    /**
     * @brief Modules can register their remote frame CanIDs.
     *
     * @param canID the can ID the module wants to register
     */
    virtual void registerOutgoingRemoteFrame(unsigned int canID);

    /**
     * @brief Modules can register the CanIDs which they are interested in.
     *
     * @param canID the can ID of frames the module is interested in
     * @param gate the input port of the module
     */
    virtual void registerIncomingDataFrame(unsigned int canID, cGate* gate);

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
     * @brief Simsignal for received data frames.
     */
    simsignal_t receivedDFPayload;

    /**
     * @brief Simsignal for received remote frames.
     */
    simsignal_t receivedRFPayload;

    /**
     * @brief Map with IDs and corresponding inputBuffer-gate of useful incoming data frames
     */
    std::map<unsigned int,cGate*> incomingDataFrameIDs;

    /**
     * @brief Map with IDs and corresponding sourceApp-gates of useful incoming remote frames
     */
    std::map<unsigned int,cGate*> outgoingDataFrameIDs;

    /**
     * @brief Vector with IDs of outgoing remote frames
     */
    std::vector<unsigned int> outgoingRemoteFrameIDs;

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
     * @brief Currently scheduled data frame
     */
    CanDataFrame *scheduledDataFrame;

    /**
     * @brief Currently scheduled error frame
     */
    ErrorFrame *scheduledErrorFrame;

    /**
     * @brief Incoming data frame is scheduled until transmission is completed.
     *
     * @param msg the incoming frame
     */
    virtual void receiveMessage(CanDataFrame *msg);

    /**
     * @brief This method generates an error message for the received frame.
     *
     * @param df the received frame
     */
    virtual void generateReceiveError(CanDataFrame *df);

    /**
     * @brief Checks whether the received frame is relevant for this node.
     *
     * @param df the received frame
     *
     * @return true if the frame is relevant, false otherwise
     */
    virtual bool checkExistence(CanDataFrame *df);

    /**
     * @brief Checks whether the data frame with the corresponding can ID is sent by this node.
     *
     * @param canID the can ID of the searched frame
     *
     * @return true if a frame with the can ID is sent by this node, false otherwise
     */
    virtual bool checkOutgoingDataFrames(unsigned int canID);

    /**
     * @brief Checks whether the remote frame with the corresponding ID is sent by this node.
     *
     * @param canID the can ID of the searched frame
     *
     * @return true if a frame with the can ID is sent by this node, false otherwise
     */
    virtual bool checkOutgoingRemoteFrames(unsigned int canID);

    /**
     * @brief Checks whether the frame is listed under the relevant incoming frames.
     *
     * @param canID the can ID of the searched frame
     *
     * @return true if a frame with the can ID is listed in the incoming frames collection, false otherwise
     */
    virtual bool checkIncomingDataFrames(unsigned int canID);

    /**
     * @brief Calculates when the frame is ready to be forwarded based on the number of bits.
     *
     * @param length length of the frame in bit
     *
     * @return the duration in seconds until frame transmission is completed
     */
    virtual double calculateScheduleTiming(int length);

    /**
     * @brief Forwards the received frame to the responsible module.
     *
     * @param msg the received frame
     */
    virtual void forwardDataFrame(CanDataFrame *msg);

    /**
     * @brief Sends the error frame to the output gate
     *
     * @param ef the generated error frame
     */
    virtual void forwardOwnErrorFrame(ErrorFrame *ef);

    /**
     * @brief Called when a error frame is received.
     *
     * @param ef the received error frame
     */
    virtual void handleExternErrorFrame(ErrorFrame *ef);

    /**
     * @brief Checks whether this node is the currently sending node.
     *
     * @return true if this node is the sending node, false otherwise
     */
    virtual bool amITheSendingNode();
};

}

#endif /* CANPORTINPUT_H_ */
