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

#ifndef FICO4OMNET_CANINPUTBUFFER_H_
#define FICO4OMNET_CANINPUTBUFFER_H_

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"
#include "fico4omnet/buffer/can/CanBuffer.h"

namespace FiCo4OMNeT {

/**
 * @brief This buffer holds messages which were received by this node.
 *
 * @ingroup Buffer
 *
 * @author Stefan Buschmann
 */
class CanInputBuffer: public CanBuffer {

public:

    /**
     * @brief Destructor
     */
    virtual ~CanInputBuffer() = default;

    /**
     * @brief Puts the frame into the collection and informs the connected gates about the receiption.
     *
     * @param msg The DataFrame to put in the buffer.
     *
     */
    virtual void putFrame(cMessage* msg);

protected:
    /**
     * @brief Initialization of the module.
     */
    virtual void initialize();

private:
    /**
     * @brief Registers the can IDs which will be received by this node.
     */
    void registerIncomingDataFramesAtPort();
};

}
#endif /* CANINPUTBUFFER_H_ */
