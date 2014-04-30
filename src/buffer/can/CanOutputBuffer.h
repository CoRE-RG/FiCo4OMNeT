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

#ifndef __FICO4OMNET_CANOUTPUTBUFFER_H_
#define __FICO4OMNET_CANOUTPUTBUFFER_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "CanBuffer.h"
#include "CanBusLogic.h"
#include "CanTrafficSourceAppBase.h"
#include "CanPortOutput.h"

namespace FiCo4OMNeT {

/**
 * @brief This buffer holds messages which will be sent to the bus.
 *
 * @ingroup Buffer
 *
 * @author Stefan Buschmann
 */
class CanOutputBuffer: public CanBuffer {

public:
    /**
     * @brief Is called by the bus logic to permit the frame transmission.
     */
    virtual void receiveSendingPermission(int id);

    /**
     * @brief Is called when the frame transmission is completed.
     */
    virtual void sendingCompleted(int id);

    /**
     * @brief Puts the frame into the collection and informs the connected gates about the receiption.
     *
     * @param frame The DataFrame to put in the buffer.
     *
     */
    virtual void putFrame(cMessage* msg);

protected:
    /**
     * @brief This method registers a frame at the bus for arbitration.
     */
    virtual void registerForArbitration(int id, bool rtr);

    /**
     * @brief Unregister from arbitration at the bus.
     */
    virtual void checkoutFromArbitration(int id);
};

}

#endif /* CANOUTPUTBUFFER_H_ */
