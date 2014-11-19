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

#ifndef __FICO4OMNET_CANBUSLOGIC_H_
#define __FICO4OMNET_CANBUSLOGIC_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <limits>
#include "CanDataFrame_m.h"
#include "ErrorFrame_m.h"
#include "FiCo4OMNeT_BusPort.h"
#include "FiCo4OMNeT_CanID.h"
#include "FiCo4OMNeT_CanOutputBuffer.h"

namespace FiCo4OMNeT {
/**
 * @brief Represents the logic of the bus. It handles the arbitration for the network and provides several statistic values.
 *
 *
 * @ingroup Bus
 *
 * @author Stefan Buschmann
 */
class CanBusLogic: public cSimpleModule {
public:
    /**
     * @brief Registers the frame of the node for the arbitration.
     */
    virtual void registerForArbitration(int id, cModule *node,
            simtime_t signInTime, bool rtr);

    /**
     * @brief The request for frame with the corresponding ID will be checked out.
     */
    virtual void checkoutFromArbitration(int id);

    int getCurrentSendingId() const {
        return currentSendingID;
    }

    int getSendingNodeID();

protected:
    /**
     * @brief collects data for the  results
     *
     * Calculates the busload, number of messages and errors.
     *
     */
    virtual void finish();

    /**
     * @brief Initializes all participants of the network
     *
     * All participants will be delivered with a message containing their vector-ID.
     * This is used to guarantee the generic behaviour of the simulation.
     *
     */
    virtual void initialize();

    /**
     * @brief Is called when a new message is received
     *
     * 1. Checks the purpose of the incoming message
     * 2. Takes an action regarding to the purpose of the message. Those may be:
     * - Arbitration messages @sa signIn(),signOut()
     * - Data-Frames or Error-Frames @sa forward_to_all(cMessage *msg)
     * - ACK-messages (will be sent to the currently sending participant)
     * - Self-messages: These are intended for organisation of the bus.
     *                  The bus notices his own state through these and organizes the arbitration mechanism with them.
     *
     */
    virtual void handleMessage(cMessage *msg);

private:

    /**
     * @brief Maximum size of an error frame.
     */
    static const int MAXERRORFRAMESIZE = 12;

    /**
     * @brief Signal for received data frames.
     */
    simsignal_t rcvdDFSignal;

    /**
     * @brief Signal for received remote frames.
     */
    simsignal_t rcvdRFSignal;

    /**
     * @brief Signal for received error frames.
     */
    simsignal_t rcvdEFSignal;

    /**
     * amount of time that the bus was in busy-state
     *
     */
    simtime_t busytime;
    /**
     * simtime in the moment of the change from state idle to state busy
     *
     */
    simtime_t busytimestamp;
    /**
     * The sign-in-time of the current data-frame. Used for forwarding to the node so that they can collect data about the elapsed time.
     *
     */
    simtime_t currsit;  //current sign in time
    /**
     * set to true if an error has occured. Prevents the sending of too many messages.
     *
     */
    bool errored;
    /**
     * During an error process this marks the position of the error in the data-frame
     *
     */
    int errpos;
    /**
     * Only if this value is set to true, the bus will try to send a new sending permission to a node
     *
     */
    bool stateok;
    /**
     * number of errors that occurred on the bus. Will be decreased if an error has benn treated. Must be 0 to let the bus carry on normally
     *
     */
    int errorcount;
    /**
     * true if a positive ACK-Message has been received from a node.
     *
     */
    bool ack_rcvd;
    /**
     * true if payload shall be used. Initialized from ned-attribute of CAN-Bus
     *
     */
    bool payload;
    /**
     * true ich ack-recognition of receiving nodes is activated. Initialized from ned-attribute of CAN-Bus
     *
     */
    bool ack;
    /**
     * @brief Bandwidth of the bus in Mbps.
     *
     */
    double bandwidth;
    /**
     * true if bus is in idle state; false if in busy state
     *
     */
    bool idle;
    /**
     * List of message-IDs that want to send a message. Used like a priority queue.
     *
     */
    std::list<CanID*> ids; //Die Nachrichten-IDs der Knoten, die senden wollen

    /**
     * Vector with CanIDs which are currently scheduled for arbitration and will be deleted after transmission.
     */
    std::vector<std::list<CanID*>::iterator> eraseids;

    //statistics:
    /**
     * number of sent data frames. For statistics-collection of the bus.
     *
     */
    long numDataFrames;

    /**
     * number of sent remote frames. For statistics-collection of the bus.
     *
     */
    long numRemoteFrames;

    /**
     * number of errors that occurred. For statistics-collection of the bus.
     *
     */
    long numErrorFrames;

    /**
     * @brief ID of the message which is currently transmitted.
     */
    int currentSendingID;

    /**
     * @brief Currently scheduled Data Frame
     */
    CanDataFrame *scheduledDataFrame;

    /**
     * @brief NodeController which is currently sending.
     */
    cModule *sendingNode;

    /**
     * @brief Sending permission for the frame with the highest priority is sent to the according node or the bus state is set to idle.
     */
    virtual void grantSendingPermission();

    /**
     * @brief Is called when the transmission of a frame is completed.
     */
    virtual void sendingCompleted();

    /**
     * @brief Is called when a data frame is received.
     *
     *
     */
    virtual void handleDataFrame(cMessage *msg);

    /**
     * @brief Is called when an error frame is received.
     *
     *
     */
    virtual void handleErrorFrame(cMessage *msg);

    /**
     * @brief Colors the connections of the bus to represent it is busy.
     */
    virtual void colorBusy();
    /**
     * @brief Colors the connections of the bus to represent it is idle.
     */
    virtual void colorIdle();
    /**
     * @brief Colors the connections of the bus to represent a transmission of an error frame.
     */
    virtual void colorError();
};

}

#endif /* CANBUSAPP_H_ */
