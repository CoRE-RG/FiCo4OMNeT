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

//FiCo4OMNeT
#include "fico4omnet/base/Defs.h"

#include "fico4omnet/bus/can/CanID.h"


//Auto-generated messages
#include "fico4omnet/linklayer/can/messages/CanDataFrame_m.h"


namespace FiCo4OMNeT {

/**
 * @brief Represents the logic of the bus. It handles the arbitration for the network and provides several statistic values.
 *
 * Since it is not effectively possible to realize the arbitration as it is specified, the bus module grants sending permissions to the node with the highest priority frame.
 * Every node has the possibility to register the frames it wants to send at the bus and wait until it is its turn to transmit the message.
 *
 * @ingroup Bus
 *
 * @author Stefan Buschmann
 */
class CanBusLogic: public cSimpleModule {

public:
    /**
     * @brief Constructor of CanBusLogic
     */
    CanBusLogic();

    /**
     * @brief Destructor of CanBusLogic
     */
    ~CanBusLogic();

    /**
     * @brief Registers the frame of the node for the arbitration.
     *
     * @param canID the ID of the can frame
     * @param node the module which wants to send the message
     * @param signInTime the time the frame was signed in
     * @param rtr identifier whether the frame is a remote frame
     */
    virtual void registerForArbitration(unsigned int canID, cModule *module,
            simtime_t signInTime, bool rtr);

    /**
     * @brief The request for frame with the corresponding ID will be checked out.
     *
     * @param canID the ID of the can frame that should be checked out
     */
    virtual void checkoutFromArbitration(unsigned int canID);

    /**
     * @brief getter for #currentSendingID
     *
     * @return the can ID of the frame that is currently transmitted.
     */
    unsigned int getCurrentSendingId() const {
        return currentSendingID;
    }

    /**
     * @brief Returns the object ID of the currently transmitting module.
     *
     * @return object ID  of the currently transmitting module
     */
    int getSendingNodeID();

protected:
    enum BusState
    {
        IDLE = 0,
        TRANSMITTING = 1
    };

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
     * @brief Signal for received frames.
     */
    simsignal_t rcvdSignal;

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
     * @brief Signal for the current state of the bus.
     */
    simsignal_t stateSignal;

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
    std::list<CanID*> ids;

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
    unsigned int currentSendingID;

    /**
     * @brief Total number of frames transmitted over the bus.
     */
    unsigned long numFramesSent;

    /**
     * @brief Total number of bits transmitted over the bus.
     */
    unsigned long numBitsSent;

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
     * @param msg received can data frame
     */
    virtual void handleDataFrame(cMessage *msg);

    /**
     * @brief Is called when an error frame is received.
     *
     * @param msg received error frame
     */
    virtual void handleErrorFrame(cMessage *msg);

    /**
     * @brief Colors the connections of the bus to represent it as busy.
     */
    virtual void colorBusy();

    /**
     * @brief Colors the connections of the bus to represent it as idle.
     */
    virtual void colorIdle();

    /**
     * @brief Colors the connections of the bus to represent a transmission of an error frame.
     */
    virtual void colorError();
};

}

#endif /* CANBUSAPP_H_ */
