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

#ifndef CANBUSAPP_H_
#define CANBUSAPP_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <limits>
#include "candataframe_m.h"
#include "arb_m.h"
#include "ack_m.h"
#include "err_m.h"
#include "BusPort.h"
#include "CanID.h"
#include "OutputBuffer.h"
//#include "CanTrafficSourceApp.h"

using namespace std;
/**
 * @brief Represents the central node of a CAN-Network, the bus
 *
 * The bus is giving the sending permission for particular messages to the respective nodes.
 * It is also forwarding all incoming data-frames and error-frames to all participating nodes.
 *
 */
class CanBusApp: public cSimpleModule {
public:
    /**
     *
     */
    virtual void registerForArbitration(int id, cModule *node, simtime_t signInTime, bool rtr);

    /**
     * @brief The request for frame with the corresponding ID will be checked out.
     */
    virtual void checkoutFromArbitration(int id);

protected:
    /**
     * @brief collects data for the  results
     *
     * Calculates the busload, number of messages and errors.
     *
     */
    virtual void finish();
    /**
     * @brief Writes a message-ID into the ids-list
     *
     * Incoming Arbitration messages ("ArbIn") trigger this method.
     * The contained message-ID is stored in the ids-list.
     * The ids-list is a substitution for the arbitration mechanism.
     *
     */
//    virtual void signIn(cMessage *msg);
    /**
     * @brief Deletes a message-ID in the ids-list
     *
     * Incoming Arbitration messages ("ArbOut") trigger this method.
     *
     */
//    virtual void signOut(cMessage *msg);
//        /**
//         * @brief Forwards an incoming Data- or Error-Frame to all participants
//         *
//         * A copy of the Data- or Error-Frame is generated and respectively sent to all participants.
//         * The original message will be deleted afterwards.
//         *
//         */
//        virtual void forward_to_all(cMessage *msg);
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
     * true if errors are activated. Initialized from ned-attribute of CAN-Bus
     *
     */
    bool errors;
    /**
     * true ich ack-recognition of receiving nodes is activated. Initialized from ned-attribute of CAN-Bus
     *
     */
    bool ack;
    /**
     * Valid values are between 10000 and 1000000. Initialized from ned-attribute of CAN-Bus
     *
     */
    int bandwidth;
    /**
     * true if bus is in idle state; false if in busy state
     *
     */
    bool idle;
    /**
     * List of message-IDs that want to send a message. Used like a priority queue.
     *
     */
    list<CanID*> ids; //Die Nachrichten-IDs der Knoten, die senden wollen

    //Statistiken:
    /**
     * number of sent messages. For statistics-collection of the bus.
     *
     */
    long numSent;   //Anzahl an verschickten Nachrichten (also alle Nachrichten)
    /**
     * number of errors that occurred. For statistics-collection of the bus.
     *
     */
    long numErr;    //Anzahl an aufgetretenen Fehlern

    /**
     * @brief ID of the message which is currently transmitted.
     */
    int currentSendingID;

    /**
     * @brief NodeController which is currently sending.
     */
    cModule *sendingNode;

    /**
     * @brief Checks wether the current frame was acknowledged or not.
     *
     *
     */
    virtual void checkAcknowledgementReception(ArbMsg *am);

    /**
     * @brief Sending permission for the frame with the highest priority is sent to the according node or the bus state is set to idle.
     *
     *
     */
    virtual void grantSendingPermission();

    /**
     * @brief Is called when an error frame is received.
     *
     *
     */
    virtual void handleErrorFrame(cMessage *msg);

    /**
     * @brief Is called when a data frame is received.
     *
     *
     */
    virtual void handleDataFrame(cMessage *msg);
};

Define_Module(CanBusApp);

#endif /* CANBUSAPP_H_ */
