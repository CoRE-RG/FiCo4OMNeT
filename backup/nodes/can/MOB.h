#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace std;
/**
 * @brief Short for "Message-Object-Buffer". Has no function as a buffer, but for storage of informations about messages 
 *
 * This object contains informations for a specific message-ID. 
 * When a node sends a data-frame to the bus, it will get the information about the message-ID from this object.
 *
 */
class MOB{
	public:
		/**
	     * @brief First constructor
		 *
		 * Called if configuration = no original stuffing bits; payload deactivated
		 *
		 * @param mode 0 = Data-Frame; 1 = Remote-Frame
		 * 
		 * @param id The message-ID of the new message
		 *
		 * @param periodicity The period of the new message in ms
		 * 
		 * @param datalength The length of the datafield (0-8 Bytes). Not needed if payload is activated.
		 *
		 * @param bs Configuration for bit-stuffing. 0 = no stuffing bits; 1 = worst case; 2 = distribution on a percentage basis; 3 = original stuffing bits except CRC 
		 *
		 * @param percentage Only important if bs = 2. Range is 0-100
		 *
		 * @param version true = CAN 2.0A (11 bit identifier); false = CAN 2.0B (29 bit identifier)
		 *
		 */
		MOB(int mode, int id, int periodicity, int datalength, int bs, int percentage, bool version);
		/**
	     * @brief Second constructor
		 *
		 * Called if configuration = original stuffing bits; payload deactivated
		 *
		 * @param mode 0 = Data-Frame; 1 = Remote-Frame
		 * 
		 * @param id The message-ID of the new message
		 *
		 * @param periodicity The period of the new message in ms
		 * 
		 * @param datalength The length of the datafield (0-8 Bytes). Not needed if payload is activated.
		 *
		 * @param bs Configuration for bit-stuffing. 0 = no stuffing bits; 1 = worst case; 2 = distribution on a percentage basis; 3 = original stuffing bits except CRC 
		 *
		 * @param percentage Only important if bs = 2. Range is 0-100
		 *
		 * @param version true = CAN 2.0A (11 bit identifier); false = CAN 2.0B (29 bit identifier)
		 *
		 * @param idbin message-ID in binary from. Used for calculations of the stuffing bits
		 * 
		 */
		MOB(int mode, int id, int periodicity, int datalength, int bs, int percentage, bool version, string idbin);
		/**
	     * @brief Third constructor
		 *
		 * Called if configuration = no original stuffing bits; payload activated
		 *
		 * @param mode 0 = Data-Frame; 1 = Remote-Frame
		 * 
		 * @param id The message-ID of the new message
		 *
		 * @param periodicity The period of the new message in ms
		 * 
		 * @param data The data of the message.
		 *
		 * @param bs Configuration for bit-stuffing. 0 = no stuffing bits; 1 = worst case; 2 = distribution on a percentage basis; 3 = original stuffing bits except CRC 
		 *
		 * @param percentage Only important if bs = 2. Range is 0-100
		 *
		 * @param version true = CAN 2.0A (11 bit identifier); false = CAN 2.0B (29 bit identifier)
		 *
		 */
		MOB(int mode, int id, int periodicity, char data[], int bs, int percentage, bool version);
		/**
	     * @brief Fourth constructor
		 *
		 * Called if configuration = original stuffing bits; payload activated 
		 *
		 * @param mode 0 = Data-Frame; 1 = Remote-Frame
		 * 
		 * @param id The message-ID of the new message
		 *
		 * @param periodicity The period of the new message in ms
		 * 
		 * @param data The data of the message.
		 *
		 * @param bs Configuration for bit-stuffing. 0 = no stuffing bits; 1 = worst case; 2 = distribution on a percentage basis; 3 = original stuffing bits except CRC 
		 *
		 * @param percentage Only important if bs = 2. Range is 0-100
		 *
		 * @param version true = CAN 2.0A (11 bit identifier); false = CAN 2.0B (29 bit identifier)
		 *
		 * @param idbin message-ID in binary from. Used for calculations of the stuffing bits
		 *
		 */
		MOB(int mode, int id, int periodicity, char data[], int bs, int percentage, bool version, string idbin);
		/**
	     * @brief Getter for the message-ID
		 *
		 * @return the message-ID 
		 *
		 */
		int getID();
		/**
	     * @brief Getter for the period of the message
		 *
		 * @return the period of the message 
		 *
		 */
		int getPeriod();
		/**
	     * @brief Getter for the length of the message
		 *
		 * @return the length of the message 
		 *
		 */
		int getLength();
		/**
	     * @brief Getter for the mode of the message
		 *
		 * @return the mode of the message. 0 = Data-Frame; 1 = Remote-Frame 
		 *
		 */
		int getMode();
		/**
	     * @brief Getter for the data of the message
		 *
		 * @return the data of the message 
		 *
		 */
		char* getData();
	private:
		/**
		* true payload shall be used.
		*
		*/
		bool payload;
		/**
		* Holds the data for the related message-ID.
		*
		*/
		char data[64];
		/**
		* Holds the binary code of the message if bit-stuffing is in original mode.
		*
		*/
		vector<char> binary;
		/**
	    * @brief Creates the binary code stored in binary.
		*
		* @param idbin the message-ID as a string
		*
		*/
		void createbinary(string idbin);
		/**
	    * @brief Calculates the number of stuffing bits of the message if original bit-stuffing mode is chosen.
		*
		* @return number of stuffing bits of the message
		*
		*/
		int binstuffbits();
		/**
	    * @brief Calculates the length of the message and writes it to length
		*
		* @param bs bit-stuffing mode
		*
		* @param percentage If bit-stuffing mode = 2 this is the percentage of estimated stuffing bits
		*
		*/
		void calc_length(int bs, int percentage);
		/**
		* Mode of the message. 0 = data-frame; 1 = remote-frame
		*
		*/
		int mode;
		/**
		* The message-ID of the Object
		*
		*/
		int id;
		/**
		* Period of the message
		*
		*/
		int periodicity;
		/**
		* Length of the data in the message
		*
		*/
		int datalength;
		/**
		* Length of the message that was calculated in calc_length
		*
		*/
		int length;
		/**
		* true = 2.0A; false = 2.0B
		*
		*/
		bool version;
};