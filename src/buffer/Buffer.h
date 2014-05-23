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

#ifndef __FICO4OMNET_BUFFER_H_
#define __FICO4OMNET_BUFFER_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

namespace FiCo4OMNeT {

/**
 * @brief Represents the content of a physical buffer on a CAN_Node 
 *
 * Nodes will make it possible to read from a buffer after receiving a related Data-Frame.
 * It is possible to write the data you wish to send into a buffer.
 *
 * @sa CAN_Node
 *
 */

class Buffer: public cSimpleModule {
public:
    /**
     * @brief This method registers the gate for the reception of the messages.
     */
    virtual void registerDestinationGate();

    /**
     *
     * @return A pointer to the DataFrame with the corresponding ID. Returns
     * null if there is no DataFrame in the buffer.
     *
     */
    cMessage* getFrame(int id);

    /**
     * @brief Puts the frame into the collection and informs the connected gates about the reception.
     *
     * @param frame The DataFrame to put in the buffer.
     *
     */
    virtual void putFrame(cMessage* frame);

    /**
     * @brief Deletes the frame with the corresponding id from the frames collection.
     */
    virtual void deleteFrame(int id);

    /**
     * @brief Forwards the frame with the corresponding id to all destination gates.
     */
    virtual void deliverFrame(int id);

    /**
     * @brief Forwards the first received frame to all destination gates.
     */
    virtual void deliverNextFrame();

protected:
    /**
     * Stores the Gates to that the messages are delivered
     */
    std::list<cGate*> destinationGates;

    /**
     * @brief Queue for the EtherFrames in the Buffer.
     */
    std::list<cMessage*> frames;

    /**
     * Signal that is emitted every time a frame was sent.
     */
    simsignal_t txPkSignal;
    /**
     * Signal that is emitted every time a frame was received.
     */
    simsignal_t rxPkSignal;

    /**
     * @brief Initialization of the module.
     */
    virtual void initialize();

    /**
     * @brief Is called when a new Frame is received in the buffer.
     *
     * When a frame is received on the in-Gate it is processed. If the destination
     * address is unspecified it is set according to the ct marker of the buffer.
     * Afterwards it is enqueued using the buffer specific enqueue(EtherFrame *newFrame)
     * method. In the end all registered receive callbacks are executed.
     *
     * @param msg The incoming message
     */
    virtual void handleMessage(cMessage *msg);

    /**
     * @brief Emits a statistics signal that a frame was sent from the buffer
     *
     * @param frame the frame that was sent
     */
    void recordPacketSent(cMessage *frame);

    /**
     * @brief Emits a statistics signal that a frame was received in the buffer
     *
     * @param frame the frame that was received
     */
    void recordPacketReceived(cMessage *frame);

    /**
     * @brief The message is delivered to the destination.
     */
    virtual void sendToDestinationGates(cMessage *msg);

private:

    /**
     * Initializes the statistics for the module
     */
    void initializeStatistics();



};

}
#endif
