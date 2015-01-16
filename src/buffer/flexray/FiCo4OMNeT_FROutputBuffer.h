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

#ifndef __FICO4OMNET_FROUTPUTBUFFER_H_
#define __FICO4OMNET_FROUTPUTBUFFER_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "FiCo4OMNeT_FRBuffer.h"
#include "../../scheduler/flexray/SchedulerMessageEvents_m.h"
#include "FiCo4OMNeT_FRScheduler.h"

namespace FiCo4OMNeT {

/**
 * @brief This buffer holds messages which will be sent to the bus.
 *
 * @ingroup Buffer
 *
 * @author Stefan Buschmann
 */
class FROutputBuffer: public FRBuffer {

public:

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
     * @brief Is called when a new Frame is received in the buffer.
     *
     * When a frame is received on the in-Gate it is processed. If the destination
     * address is unspecified it is set according to the ct marker of the buffer.
     * Afterwards it is enqueued using the buffer specific enqueue(EtherFrame *newFrame)
     * method. In the end all registered receive callbacks are executed.
     *
     * @param msg The incoming message
     */
    virtual void handleMessage(cMessage *msg);
};

}

#endif /* FROUTPUTBUFFER_H_ */
