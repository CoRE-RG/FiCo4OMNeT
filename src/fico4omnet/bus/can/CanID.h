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

#ifndef __FICO4OMNET_CANID_H_
#define __FICO4OMNET_CANID_H_

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"

namespace FiCo4OMNeT {

/**
 * @brief Used by the #CanBusLogic for the arbitration of can frames.
 *
 * This object contains informations about specific message-ID's.
 * The bus can obtain informations about the related node of a message-ID and the configuration of that message-ID. 
 *
 * @author Stefan Buschmann
 */
class CanID {

	public:
		/**
	     * @brief Constructor
		 *
		 * Creates an ID object
		 *
		 * @param canID the ID of the can frame
		 * @param node the module which wants to send the message
		 * @param signInTime the time the frame was signed in
		 * @param rtr identifier whether the frame is a remote frame
		 */
		CanID(unsigned int canID, cModule* module, simtime_t signInTime, bool rtr);

		/**
	     * @brief Getter for the can ID
		 *
		 * @return the can ID of the object
		 *
		 */
		unsigned int getCanID() const;

		/**
	     * @brief Getter for the node of the message
		 *
		 * @return the related node for this CanID object
		 *
		 */
		cModule* getNode() const;

		/**
	     * @brief Getter for signInTime
		 *
		 * @return the time, when the node first tried to send the message to the bus 
		 *
		 */
		simtime_t getSignInTime() const;

		/**
	     * @brief Getter for the RTR-bit of the CanID object
		 *
		 * @return the RTR-bit of the CanID object.
		 *
		 */
		bool getRtr() const;

	private:
		/**
		* @brief the can ID
		*/
		unsigned int canID;

		/**
		* @brief Related module which wants to send the message
		*/
		cModule *module;

		/**
		* @brief Point of time when the frame was signed in
		*/
		simtime_t signInTime;

		/**
		* @brief identifier whether the frame is a data or remote frame.
		*/
		bool rtr;
};

}
#endif
