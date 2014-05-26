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

#ifndef __FICO4OMNET_CANTRAFFICSOURCEAPPBASE_H_
#define __FICO4OMNET_CANTRAFFICSOURCEAPPBASE_H_

#include <omnetpp.h>
#include <string.h>
#include "FiCo4OMNeT_CanBuffer.h"
#include "FiCo4OMNeT_CanPortInput.h"
#include "CanDataFrame_m.h"

namespace FiCo4OMNeT {

/**
 * @brief Traffic source application used to generate outgoing messages.
 *
 * @ingroup Applications
 *
 * @author Stefan Buschmann
 */
class CanTrafficSourceAppBase: public cSimpleModule {

protected:
    /**
     * @brief Initialization of the module.
     */
    virtual void initialize();

    /**
     * @brief This method checks whether the configured values from the ini files are permitted.
     */
    virtual void checkParameterValues();

    /**
     * @brief Self messages are processed and outgoing frames are built.
     *
     * @param msg incoming self message
     */
    virtual void handleMessage(cMessage *msg);

private:
    /**
     * @brief Overhead for a data frame.
     *
     * The variable includes Start of frame, arbitration field (11 Bit), remote transmission bit,
     * identifier extension bit, reserved bit, data length code, CRC, CRC-delimiter, ACK-slot,
     * ACK-delimiter, end of frame and inter frame space.
     */
    static const int DATAFRAMECONTROLBITS = 47;

    /**
     * @brief Additional bit needed for the 29 bit ID.
     */
    static const int ARBITRATIONFIELD29BIT = 20;

    /**
     * @brief Number of control bits which are used in bit-stuffing. Just 34 of the 47 are subject to bit-stuffing.
     */
    static const int CONTROLBITSFORBITSTUFFING = 34;

    /**
     * @brief Maximum ID value for Version 2.0A.
     */
    static const int VERSIONAMAX = 2047;
    /**
     * @brief Maximum ID value for Version 2.0B.
     */
    static const int VERSIONBMAX = 536870911;

    /**
     * @brief The version of CAN used in this network. 2.0A or 2.0B
     */
    std::string canVersion;

    /**
     * @brief Value for the percentage distribution for bit stuffing. Valid values 0 to 1.
     */
    double bitStuffingPercentage;

    /**
     * @brief Creates a data frame which will be queued in the buffer.
     */
    void initialRemoteFrameCreation();

    /**
     * @brief Registers the outgoing remote frame at the port.
     */
    void registerRemoteFrameAtPort(int canID);

    /**
     * @brief Collection including all
     */
    std::vector<CanDataFrame*> outgoingDataFrames;

    /**
     * @brief Creates a data frame which will be queued in the buffer.
     */
    void initialDataFrameCreation();

    /**
     * @brief Registers the outgoing data frames at the port to receive incoming remote frames.
     */
    void registerDataFrameAtPort(int canID);

    /**
     * @brief Checks whether the CAN-ID matches the restrictions of the CAN version.
     */
    int checkAndReturnID(int id);

    /**
     * @brief Calculates the length for the data frame.
     */
    int calculateLength(int datalength);

    /**
     * @brief Calculates the additional bits needed for the chosen bitstuffing method.
     */
    int calculateStuffingBits(int dataLength, int arbFieldLength);

    /**
     * @brief Transmits the data frame to the connected output buffer.
     */
    void dataFrameTransmission(CanDataFrame *df);
};
}
#endif /* CANTRAFFICSOURCEAPPBASE_H_ */
