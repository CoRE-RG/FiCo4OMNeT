#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

/**
 * @brief Represents the content of a physical buffer on a CAN_Node 
 *
 * Nodes will make it possible to read from a buffer after receiving a related Data-Frame.
 * It is possible to write the data you wish to send into a buffer.
 *
 * @sa CAN_Node
 *
 */
class Buffer{
	public:
		/**
	     * @brief Constructor
		 *
		 * Creates the buffer
		 *
		 * @param id The message-ID which the buffer will be associated to
		 *
		 * @param mode 0 = Send Message; 1 = Receive Message
		 *
		 */
		Buffer(int id, int mode);
		/**
	     * @brief Getter for the message-ID 
		 *
		 * @return the message-ID of the buffer 
		 *
		 */
		int getId();
		/**
	     * @brief Getter for the message-ID 
		 *
		 * @return the message-ID of the object 
		 *
		 */
		int getMode();
		/**
	     * @brief Setter for the data of the buffer 
		 *
		 * This sets the data of this buffer to the input-data
		 *
		 * @param datain data in the buffer will be set to this
		 *
		 */
		void setData(char datain[]);
		/**
	     * @brief Getter for the data of the buffer 
		 *
		 * @return the data as a char-pointer 
		 *
		 */
		char* getData();

	private:
		/**
		* The data stored in the buffer
		*
		*/
		char data[8];
		/**
		* message-ID
		*
		*/
		int id;
		/**
		* Buffer-Mode. 0 = Send; 1 = Receive
		*
		*/
		int mode; //Empfang oder Versendung
};