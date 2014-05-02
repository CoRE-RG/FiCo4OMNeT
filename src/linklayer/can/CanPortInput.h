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

#ifndef __FICO4OMNET_CANPORTINPUT_H_
#define __FICO4OMNET_CANPORTINPUT_H_

#include <omnetpp.h>
#include <string.h>
#include "CanDataFrame_m.h"
#include "ErrorFrame_m.h"
#include "CanPortOutput.h"

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

    virtual void finish();

    /**
     * @brief Handles all received messages
     *
     * @param msg the incoming message.
     */
    virtual void handleMessage(cMessage *msg);

private:

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
     * if errors = true then this is the amount of errors appearing on the node in percent. Initialized from ned-attribute of CAN-Node
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
     * @brief
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
     * @brief
     */
    virtual void handleExternErrorFrame(ErrorFrame *ef);
};

}

#endif /* CANPORTINPUT_H_ */
