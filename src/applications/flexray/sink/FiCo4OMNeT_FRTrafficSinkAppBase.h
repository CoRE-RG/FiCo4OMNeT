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

#ifndef __FICO4OMNET_FRTRAFFICSINKAPPBASE_H_
#define __FICO4OMNET_FRTRAFFICSINKAPPBASE_H_

#include <omnetpp.h>
#include <string.h>
#include "FiCo4OMNeT_FRInputBuffer.h"
#include "FRFrame_m.h"

namespace FiCo4OMNeT {

/**
 * @brief Traffic sink application used to handle incomming messages.
 *
 * @ingroup Applications
 *
 * @author Stefan Buschmann
 */
class FRTrafficSinkAppBase : public cSimpleModule{

protected:
    /**
     * @brief Initialization of the module.
     */
    virtual void initialize();

    /**
     * @brief Collects incoming message and writes statistics.
     *
     * @param msg incoming frame
     */
    virtual void handleMessage(cMessage *msg);

private:
    /**
     * @brief Number of messages currently in the Buffer
     */
    int bufferMessageCounter;

    /**
     * @brief Shows whether the application is working or not.
     */
    bool idle;

    /**
     * @brief The CAN-ID of the message which is currently processed.
     */
    int currentFrameID;

    /**
     * @brief Requests a frame from buffer.
     */
    void requestFrame();

    /**
     * @brief The sink processes the frame.
     *
     * @param workTime represents the time it takes until the sink can process the next frame.
     */
    void startWorkOnFrame(float workTime);
};

}

#endif /* FRTRAFFICSINKAPP_H_ */
