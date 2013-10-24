#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "dataframe_m.h"

/**
 * @brief Represents the content of a physical buffer on a CAN_Node 
 *
 * Nodes will make it possible to read from a buffer after receiving a related Data-Frame.
 * It is possible to write the data you wish to send into a buffer.
 *
 * @sa CAN_Node
 *
 */

//using namespace std;
class Buffer : public cSimpleModule {
public:
    /**
     *
     * @return A pointer to the DataFrame with the corresponding ID. Returns
     * null if there is no DataFrame in the buffer.
     *
     */
    DataFrame* getFrame(int id);

    /**
     * @brief Puts the frame into the collection and informs the connected gates about the receiption.
     *
     * @param frame The DataFrame to put in the buffer.
     *
     */
    void putFrame(DataFrame* frame);

    /**
     * @brief Deletes the frame with the corresponding id from the frames collection.
     */
    void deleteFrame(int id);

    /**
     * @brief Forwards the frame with the corresponding id to all destination gates.
     */
    void deliverFrame(int id);

    /**
     * @brief Forwards the frame with the highest priority to all destination gates.
     */
    void deliverPrioFrame();

    /**
     * @brief Forwards the first received frame to all destination gates.
     */
    void deliverNextFrame();

protected:
    /**
     * Stores the Gates to that the messages are delivered
     */
    std::list<cGate*> destinationGates;

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
    void handleMessage(cMessage *msg);

private:
    /**
     * @brief Queue for the EtherFrames in the Buffer.
     */
    std::list<DataFrame*> frames;

    /**
     *
     */
    void sendToDestinationsGates(DataFrame *msg);
};

Define_Module(Buffer);
