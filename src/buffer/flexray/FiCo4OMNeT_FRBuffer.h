#ifndef __FICO4OMNET_FRBUFFER_H_
#define __FICO4OMNET_FRBUFFER_H_

#include <stdio.h>
#include <string.h>
#include "omnetpp.h"
#include "FiCo4OMNeT_Buffer.h"
#include "FRFrame_m.h"

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

/**
 * @brief Base class for the input and output buffer for CAN nodes.
 *
 * @ingroup Buffer
 *
 * @author Stefan Buschmann
 */
class FRBuffer : public Buffer {
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
    FRFrame* getFrame(int frameId);

    /**
     * @brief Puts the frame into the collection and informs the connected gates about the receiption.
     *
     * @param msg The FRFrame to put in the buffer.
     *
     */
    virtual void putFrame(cMessage* msg);

    /**
     * @brief Deletes the frame with the corresponding id from the frames collection.
     */
    virtual void deleteFrame(int frameId);

    /**
     * @brief Forwards the frame with the corresponding id to all destination gates.
     */
    virtual void deliverFrame(int frameId);

    /**
     * @brief Forwards the frame with the highest priority to all destination gates.
     */
    virtual void deliverPrioFrame();

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
    std::list<FRFrame*> frames;

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
//    virtual void handleMessage(cMessage *msg);

private:

    /**
     *
     */
//    virtual void sendToDestinationGates(FRFrame *msg);
};

}

#endif
