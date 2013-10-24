#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <cmodule.h>

//using namespace std;

/**
 * @brief Used for the arbitration list of the CAN_Bus node. 
 *
 * This object contains informations about specific message-ID's.
 * The bus can obtain informations about the related node of a message-ID and the configuration of that message-ID. 
 *
 */
class CanID {
	public:
		/**
	     * @brief Constructor
		 *
		 * Creates an ID object
		 *
		 */
		CanID(int id, cModule* node, simtime_t signInTime, bool rtr, bool remotesent);
		/**
	     * @brief Getter for the message-ID 
		 *
		 * @return the message-ID of the object 
		 *
		 */
		int getId();
		/**
	     * @brief Getter for the node of the message
		 *
		 * @return the related node of this message-ID
		 *
		 */
		cModule* getNode();
		/**
	     * @brief Getter for signInTime
		 *
		 * @return the time, when the node first tried to send the message to the bus 
		 *
		 */
		simtime_t getSignInTime();
		/**
	     * @brief Getter RTR-bit of the message-ID
		 *
		 * @return the RTR-bit of the message-ID. 0 = Data-Frame; 1 = Remote-Frame
		 *
		 */
		bool getRtr();
		/**
	     * @brief Getter for the bool remoteSent
		 *
		 * @return remoteSent. If true = node won't re-schedule the message after this. If false = message will be re-scheduled.
		 *
		 */
		bool getRemotesent();
	private:
		/**
		* message-ID
		*
		*/
		int id;
		/**
		* Related node to this ID
		*
		*/
		cModule *node;
		/**
		* Point of time the node started to send an arbitration-message
		*
		*/
		simtime_t signInTime;
		/**
		* false = data-frame; true = remote-frame
		*
		*/
		bool rtr;
		/**
		* true if a message is sent upon a remote-request from another node
		*
		*/
		bool remotesent;
};
