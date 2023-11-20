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

#ifndef FICO4OMNET_BUFFER_H_
#define FICO4OMNET_BUFFER_H_

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"

namespace FiCo4OMNeT {

using namespace omnetpp;

/**
 * @brief Represents the content of a physical buffer.
 *
 * Nodes can use the buffers to store incoming or outgoing frames.
 *
 * @author Stefan Buschmann
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
     * @brief Searches the buffer for a specific frame.
     *
     * @param objectId The unique identifier for the frame that should be searched.
     *
     * @return A pointer to the DataFrame with the corresponding object identifier. Returns
     * null if there is no DataFrame in the buffer.
     *
     */
    cMessage* getFrame(long objectId);

    /**
     * @brief Puts the frame into the collection #frames.
     *
     * @param frame The frame to put in the buffer.
     *
     */
    virtual void putFrame(cMessage* frame);

    /**
     * @brief Deletes the frame with the corresponding object identifier from the collection #frames.
     *
     * @param objectId The unique identifier for the frame that should be deleted.
     */
    void deleteFrame(int objectId);

    /**
     * @brief Forwards the frame with the corresponding id to all destination gates.
     *
     * @param objectId The unique identifier for the frame that should be delivered.
     */
    void deliverFrame(int objectId);

    /**
     * @brief Forwards the first received frame to all destination gates.
     */
    virtual void deliverNextFrame();

protected:

    ~Buffer();
    
    /**
     * @brief Caches the queuesize sum of all bytes of frames
     * increased with incoming frame, decreased with outgoing frame
     */
    size_t queueSize;
    /**
     * @brief Signal containing the queue length, that is emitted every time a frame
     * was inserted or removed.
     */
    static simsignal_t queueLengthSignal;
    /**
     * @brief Signal containing the queue size in byte, that is emitted every time a frame
     * was inserted or removed.
     */
    static simsignal_t queueSizeSignal;

    /**
     * Stores the Gates to which the messages are delivered.
     */
    std::list<cGate*> destinationGates;

    /**
     * @brief Collection for the frames in the Buffer.
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
     * @brief Is called when a new frame is received.
     *
     * When a frame is received on the in- or directIn-Gate it will be processed.
     *
     * @param msg The incoming message
     */
    virtual void handleMessage(cMessage *msg);

    /**
     * @brief Emits a statistics signal that a frame was sent from the buffer.
     *
     * @param frame the frame that was sent
     */
    void recordPacketSent(cMessage *frame);

    /**
     * @brief Emits a statistics signal that a frame was received by the buffer.
     *
     * @param frame the frame that was received
     */
    void recordPacketReceived(cMessage *frame);

    /**
     * @brief The message is delivered to the destination.
     *
     * @param msg the message to be sent
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
