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
// 

#ifndef FICO4OMNET_CANCOLOUREDSOURCEAPPE_H_
#define FICO4OMNET_CANCOLOUREDSOURCEAPPE_H_

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"
#include "fico4omnet/applications/can/source/CanTrafficSourceAppBase.h"
//Auto-generated messages
#include "fico4omnet/linklayer/can/messages/CanDataFrame_m.h"

namespace FiCo4OMNeT {

/**
 * @brief Traffic source application used to generate outgoing data and remote frames.
 *
 * @ingroup Applications
 *
 * @author Stefan Buschmann
 */
class CanColouredSourceApp:  public virtual CanTrafficSourceAppBase{

public:
    /**
     * @brief Constructor of CanTrafficSourceAppBase
     */
    CanColouredSourceApp();

    /**
     * @brief Destructor of CanTrafficSourceAppBase
     */
    ~CanColouredSourceApp();

protected:
    /**
     * @brief Initialization of the module.
     *
     * All data and remote frames this node can send are created within the initialization. See #initialDataFrameCreation() and #initialRemoteFrameCreation() for further information.
     *
     */
    virtual void initialize(int stage);

    /**
     * @brief Incoming messages are processed.
     *
     * See #frameTransmission(CanDataFrame *df) for further information.
     *
     * @param msg incoming self message
     */
    virtual void handleMessage(omnetpp::cMessage *msg);

private:
    /**
     * @brief Holds the display string configured in the ini file.
     */
    const char* frameDisplayString;

    /**
     * @brief Transmits a data or remote frame to the connected output buffer.
     *
     * @param df the frame that should be sent
     */
    void frameTransmission(CanDataFrame *df);
};
}
#endif /* CANCOLOUREDSOURCEAPPE_H_ */
