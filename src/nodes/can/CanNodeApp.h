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

#ifndef CANNODEAPP_H_
#define CANNODEAPP_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "dataframe_m.h"
#include "arb_m.h"
#include "ack_m.h"
#include "err_m.h"
#include "MOB.h"
#include "Buffer.h"
#include "Stats.h"
#include "NodePort.h"

using namespace std;

class canNodeApp : public cSimpleModule{
private:
    /**
    * Internally used. ID of the inout-gate array of the bus
    *
    */
    int vectorid;
    /**
    * true if send-buffer is full --> buffer configured to send all message-ids
    *
    */
    bool sendfull;
    /**
    * true if get-buffer is full --> buffer configured to receive all message-ids
    *
    */
    bool getfull;
    /**
    * true if external code shall be used. Initialized from ned-attribute of CAN-Node
    *
    */
    bool extcode;
    /**
    * Number of current errors. Will be decreased after an error has been treated. If this value is 0, the node will carry on normally.
    *
    */
    int errocc;
    /**
    * true if payload shall be used. Initialized from ned-attribute of CAN-Node
    *
    */
    bool payload;
    /**
    * true for 2.0A; false for 2.0B. Initialized from ned-attribute of CAN-Node
    *
    */
    bool version;       //true = 2.0A; false = 2.0B
    /**
    * @brief Percentage used for amount of stuffing bits in case of bitstuffing mode 2. Initialized from ned-attribute of CAN-Node
    *
    * values valid: 0-100
    */
    int percentage;
    /**
    * @brief Bit-Stuffing mode used. Initialized from ned-attribute of CAN-Node
    *
    * 0 = no stuffing bits
    * 1 = worst case. Maximum amount of stuffing bits
    * 2 = distribution. percentage is responsible for the amount of stuffing bits
    * 3 = original. Calculated number of stuffing bits based on the binary code of the related message. CRC is not included here.
    *
    */
    int bitstuffing;
    /**
    * true if errors are activated. Initialized from ned-attribute of CAN-Node
    *
    */
    bool errors;
    /**
    * if errors = true then this is the amount of errors appearing on the node in percent. Initialized from ned-attribute of CAN-Node
    *
    */
    int errorperc;
    /**
    * true ich ack-recognition of receiving nodes is activated. Initialized from ned-attribute of CAN-Node
    *
    */
    bool ack;
    /**
    * Valid values are between 10000 and 1000000. Initialized from ned-attribute of CAN-Node
    *
    */
    int bandwidth;

    /**
    * @brief Initializes the traffic-generator
    *
    * Called upon start of the simulation and executed with attributes from the traffic-generator in omnetpp.ini.
    *
    */
    virtual void trafficGenInit();
    /**
    * @brief Called when node sends a data-frame to the bus
    *
    * The node sends a data-frame to the bus upon getting the permission
    *
    * The list out_m is used to generate the specific data-frame.
    * Afterwards, the message will be re-scheduled.
    *
    * @param msg cMessage-Object that holds information about the next Data-Frame that is scheduled
    *
    *
    */
    virtual void sendDFrame(cMessage *msg);
    /**
    * @brief Called when the node receives a data-frame
    *
    * An incoming data-frame is processed through this method
    *
    * The method will parse the message to an Data-Frame-Object and takes actions according to the omnetpp.ini-configuration
    *
    * @param msg cMessage-Object that will be transformed into a Data-Frame
    *
    */
    virtual void handleDFrame(cMessage *msg);
    /**
    * @brief Called when a remote-frame is recognized by the node
    *
    * The node will search for the desired message to schedule it at immediately for sending
    *
    * @param msg cMessage-Object that holds informations about the remote-frame
    *
    */
    virtual void remoteRequest(cMessage *msg);

    //Statistiken:
    /**
    * Current send time. Used for statistics collection
    *
    */
    simtime_t currst; //current send time
    /**
    * number of sent messages. Used for statistics collection
    *
    */
    long numSent;   //Anzahl verschickter Nachrichten
    /**
    * number of messages that were interesting for this node. Used for statistics collection
    *
    */
    long rcvdInteresting;   //Nachrichten, die f�r diesen Knoten interessant waren
    /**
    * vector of Stats-Objects related to (received) message-IDs. Used for statistics collection
    *
    */
    vector<Stats*> messagestats;

    /**
    * @brief Called if an interesting message is received. For statistics collection
    *
    * The time that the incoming message has taken to be delivered from the sender to this node is added to the Stats-Object in the vector messagestats
    *
    * @param id message-ID of the related Stats-Object
    *
    * @param timetaken elapsed time since the sending wish of the sender-node
    *
    */
    virtual void newTime(int id, simtime_t timetaken);  //Wenn eine Nachricht ankommt und die �bertragungsdauer eingepflegt wird

    //Message-Buffer:
    /**
    * number of used physical message-buffers
    *
    */
    unsigned int numOfMobs;
    /**
    * vector for Buffer-Objects. These buffers are the in/out-mechanism of the node for the incoming and outgoing data in the messages.
    *
    */
    vector<Buffer*> phys_mobs;  //physikalisch vorhandene Message-Object-Buffer
    /**
    * @brief Called when a message will be sent.
    *
    * Reads the data of the Buffer in phys_mobs to send it in the related message.
    *
    * @param id message-ID
    *
    * @return the data of the buffer which will be forwarded to the outgoing message
    *
    */
    char* internalGetBufferData(int id);
    /**
    * @brief Called upon an incoming message.
    *
    * Writes the data of the incoming messages to the related Buffer in phys_mobs
    *
    * @param id message-ID
    *
    * @param datain data of the incoming message
    *
    * @return 0 if successful; -1 if not successful
    *
    */
    int internalSetBufferData(int id, char datain[]);

    //omnetpp.ini traffic-gen:
    /**
    * Outgoing messages. vector of message-IDs that are assigned to this node. Remote-frames are included.
    *
    */
    vector<MOB*> out_m; //Diesem Knoten zugeordnete Nachrichten-IDs; auch Remote-Frames
    /**
    * Incoming messages. Vector of message-IDs that are assigned to this node.
    *
    */
    vector<int> in_m;   //Diese IDs empfaengt der Knoten
protected:
    /**
     * @brief Called when simulation ended
     *
     * Calls collectStats().
     *
     */
    virtual void finish();
    /**
     * @brief Collects data for the  results
     *
     * Finishes the collection of statistics and writes them into a scalar-file.
     * The statistics are collected for every message-ID. They contain:
     * - End-to-End-latency (min, max, jitter, mean)
     * - For the node: Sent Messages, Interesting received messages
     *
     */
    virtual void collectStats();
    /**
     * @brief Generates a binary version of the id
     *
     * The incoming id is transformed into a string containing the binary order of the number.
     * This string is later used for the calculation of stuffing bits
     *
     * @param id The transformed id
     *
     * @sa MOB
     *
     * @return message-ID as a string containing '0' for 0 and '1' for 1
     *
     */
    virtual string idbinary(int id);
    /**
     * @brief Initializes the node and may call the trafficGenInit
     *
     * Initialization of parameters and of the traffic generator if it is enabled.
     *
     */
    virtual void initialize();
    /**
     * @brief Called upon incoming messages
     *
     * Self messages are intended for the internal organisation of the node.
     * Incoming messages from the bus can trigger other methods as sendDFrame(), handleDFrame() or lead to appropriate actions taken by the node.
     * Initializes the node for proper functionality of the communication with the bus upon receiving init-messages.
     *
     */
    virtual void handleMessage(cMessage *msg);
public:
    /**
     * @brief Modifies the setup of the physical buffers (adding a message-ID)
     *
     * The list of the physical buffers is changed upon calling this method.
     *
     * @param id The message-ID should be written into the list of the physical buffers
     *
     * @param mode 0 = Sent Message; 1 = Received Message
     *
     * @return 1 if the buffers are set to send or receive all messages (depending on value of mode), 0 if successful, -1 if not successful
     *
     */
    int add_mob(int id, int mode);
    /**
     * @brief Modifies the setup of the physical buffers (removing a message-ID)
     *
     * The list of the physical buffers is changed upon calling this method.
     *
     * @param id This message-ID will be removed from the physical buffers
     *
     * @return 0 if successful, -1 if there was no such message-ID to remove
     *
     */
    int remove_mob(int id);
    /**
     * @brief Read on a physical buffer
     *
     * The method will deliver the data-content of a buffer with this message-ID.
     * Not to confuse with "internalGetBufferData"! This method is only designed to read buffers which are set to receive messages.
     *
     * @param id The data of the buffer with this message-ID will be returned.
     *
     * @return the data of the buffer
     *
     */
    char* getBufferData(int id);
    /**
     * @brief Modifies the data on a physical buffer
     *
     * The buffer associated to this message-ID will be written on with the input-data
     *
     * @param id The id of the designated buffer
     *
     * @param datain The incoming data which will be written to the data field of the designated buffer
     *
     * @return 0 if successful, -1 if not successful
     *
     */
    int setBufferData(int id, char datain[]);
    /**
     * @brief Checks the functionality of some units
     *
     * Checks specific units of the CAN-Bus-simulation.
     * Devices:
     * - 1: message-ID to binary. Input message-ID is 431668590, output binary should be 11001101110101011110101101110
     * - 2: Incoming buffer test. An incoming buffer is created. Data is written on it and read out immediately. Calls internalSetBufferData(int id, char datain[]) and getBufferData(int id)
     * - 3: Outgoing buffer test. An outgoing buffer is created. Data is written on it and read out immediately. Calls setBufferData(int id, char datain[]) and internalGetBufferData(int id)
     * - 4: Stats test. 10 Values are put into an stats object. This checks the correct calculations of the stats object.
     * - 5: Original bit stuffing test. Input data and id are chosen to cause many stuffing bits (19). Checks if this is correctly calculated.
     *
     * @param unitId The number of the device you wish to test
     *
     * @return 0 if successful, -1 if not successful, -2 if wrong id
     *
     */
    int checkUnits(int unitId);
};

Define_Module(canNodeApp);

#endif /* CANNODEAPP_H_ */
