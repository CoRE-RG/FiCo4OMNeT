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

#ifndef __FICO4OMNET_FRPORTINPUT_H_
#define __FICO4OMNET_FRPORTINPUT_H_

//FiCo4OMNeT
#include "fico4omnet/base/Defs.h"

//Auto-generated messages
#include "fico4omnet/linklayer/flexray/messages/FRFrame_m.h"

namespace FiCo4OMNeT {

/**
 * @brief Received messages are initially handled in this module.
 *
 * @ingroup Port
 *
 * @author Stefan Buschmann
 */
class FRPortInput: public cSimpleModule{
public:
    /**
     * @brief Constructor
     */
    FRPortInput();
    
protected:
    /**
     *
     */
    virtual void initialize();

    /**
     * @brief Handles all received messages
     *
     * @param msg the incoming message.
     */
    virtual void handleMessage(cMessage *msg);

private:
    /**
     * @brief Bandwidth of the network.
     */
    double bandwidth;

    /**
     * @brief Simsignal for received static frames.
     */
    simsignal_t rcvdSFSignal;

    /**
     * @brief Simsignal for received dynamic frames.
     */
    simsignal_t rcvdDFSignal;

    /**
     * @brief Handles the received message.
     */
    virtual void receivedExternMessage(FRFrame *msg);

    /**
     * @brief Calculates when the frame is ready to be forwarded based on the number of bits.
     */
    virtual double calculateScheduleTiming(int length);
};

}
#endif /* FRPORTINPUT_H_ */
