#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

/**
 * @brief This object is created for every message-ID that is received and processed by a CAN_Node
 *
 * Everytime a message with this message-ID reaches a node, he will transmit the time taken for the sending into this object.
 * It collects statistics over maximum, minimum and average sending time of that message.
 *
 * @sa CAN_Node
 *
 */
class Stats{
	public:
		/**
	     * @brief Constructor
		 *
		 * Creates a Stats-Object. This will only be called, if the node is designed to be interested in this particular message.
		 *
		 * @param id The message-ID which the the Stats-object will be associated to.
		 *
		 */
		Stats(int id);
		/**
	     * @brief Called when a Data-Frame with this message-ID arrives and is sent completely
		 *
		 * Transmits the time taken for this sending procedure into this object.
		 * The method will automatically calculate the current max, min and average send time.
		 *
		 * @param time The time the message needed to get fully transmitted over the bus
		 *
		 */
		void newTime(simtime_t time);
		/**
	     * @brief Getter for maximum sending time 
		 *
		 * @return the maximum sending time
		 *
		 */
		simtime_t getMax();
		/**
	     * @brief Getter for minimum sending time 
		 *
		 * @return the minimum sending time
		 *
		 */
		simtime_t getMin();
		/**
	     * @brief Getter for average sending time 
		 *
		 * @return the average sending time
		 *
		 */
		simtime_t getMean();
		/**
	     * @brief Getter for the message-ID 
		 *
		 * @return the message-ID
		 *
		 */
		int getId();
	private:
		/**
		* true if the Stats-Object has been initialized
		*
		*/
		bool init;
		/**
		* message-ID of the Stats-Object
		*
		*/
		int id;
		/**
		* number of messages that were used for statistics-collection
		*
		*/
		int numProcessed;
		/**
		* Maximum elapsed time of the message
		*
		*/
		simtime_t max;
		/**
		* Minimum elapsed time of the message
		*
		*/
		simtime_t min;
		/**
		* Average elapsed time of the message
		*
		*/
		simtime_t currMean;
		/**
		* Total elapsed time of the message. Used for calculating the average time.
		*
		*/
		simtime_t totalTime;
};
