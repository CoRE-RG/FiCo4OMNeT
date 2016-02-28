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

#ifndef __FICO4OMNET_CANBUFFER_H_
#define __FICO4OMNET_CANBUFFER_H_

//FiCo4OMNeT
#include "fico4omnet/base/Defs.h"
#include "fico4omnet/buffer/Buffer.h"

//Auto-generated messages
#include "fico4omnet/linklayer/can/messages/CanDataFrame_m.h"

namespace FiCo4OMNeT {

/**
 * @brief Base class for the input and output buffer for CAN nodes.
 *
 * @ingroup Buffer
 *
 * @author Stefan Buschmann
 */
class CanBuffer : public Buffer {

public:
    /**
     * @brief Searches the collection for a frame with a specific can ID.
     *
     * @param canID The can ID of the frame that should be searched in the buffer.
     *
     * @return A pointer to the DataFrame with the corresponding can ID. Returns
     * null if there is no DataFrame in the buffer.
     *
     */
    CanDataFrame* getFrame(unsigned int canID);

    /**
     * @brief Puts the can frame into the collection.
     *
     * @param msg The frame to put in the buffer.
     *
     */
    virtual void putFrame(cMessage* msg);

    /**
     * @brief Deletes the frame with the corresponding can id from the #frames collection.
     */
    void deleteFrame(unsigned int canID);

    /**
     * @brief Deletes the frame from the #frames collection.
     */
    void deleteFrame(CanDataFrame* frame);

    /**
     * @brief Forwards the frame with the corresponding can id to all destination gates.
     */
    void deliverFrame(unsigned int canID);

    /**
     * @brief Forwards the frame with the highest priority to all destination gates.
     */
    virtual void deliverPrioFrame();

    /**
     * @brief Forwards the first received frame to all destination gates.
     */
    virtual void deliverNextFrame();

    /**
     * @brief Returns the frame which is currently transmitted.
     *
     * @return the frame which is currently transmitted
     */
    virtual CanDataFrame* getCurrentFrame();

protected:
    /**
     * @brief Stores the Gates to that the messages are delivered
     */
    std::list<cGate*> destinationGates;

    /**
     * @brief Queue for the EtherFrames in the Buffer.
     */
    std::list<CanDataFrame*> frames;

    /**
     * @brief If the message object buffer (MOB) is true frames with the same ID will be overwritten.
     */
    bool MOB;

    /**
     * @brief Currently delivered frame.
     */
    CanDataFrame* currentFrame;

    /**
     * @brief Initialization of the module.
     */
    virtual void initialize();
};

}
#endif
