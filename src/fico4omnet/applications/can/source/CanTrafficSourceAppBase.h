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

#ifndef FICO4OMNET_CANTRAFFICSOURCEAPPBASE_H_
#define FICO4OMNET_CANTRAFFICSOURCEAPPBASE_H_

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"
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
class CanTrafficSourceAppBase: public omnetpp::cSimpleModule {

public:
    /**
     * @brief Constructor of CanTrafficSourceAppBase
     */
    CanTrafficSourceAppBase();

    /**
     * @brief Destructor of CanTrafficSourceAppBase
     */
    virtual ~CanTrafficSourceAppBase();

protected:
    /**
     * @brief Initialization of the module.
     *
     * All data and remote frames this node can send are created within the initialization. See #initialDataFrameCreation() and #initialRemoteFrameCreation() for further information.
     *
     */
    virtual void initialize(int stage);

    /**
     * @brief Number of initialization stages.
     */
    virtual int numInitStages() const { return 3; }

    /**
     * @brief This method checks whether the configured values from the ini files are permitted.
     */
    virtual void checkParameterValues();

    /**
     * @brief Incoming messages are processed.
     *
     * See #frameTransmission(CanDataFrame *df) for further information.
     *
     * @param msg incoming self message
     */
    virtual void handleMessage(omnetpp::cMessage *msg);

    /**
     * @brief Calculates the length for the data frame.
     *
     * The size needed for the arbitration field, the control bits and the stuffing bits are added up.
     *
     * @param dataLength Size of the data field in bytes
     *
     * @return Returns the size for the can frame without the size needed for the data field.
     */
    unsigned int calculateLength(unsigned int dataLength);

    /**
     * @brief Simsignal for received data frames.
     */
    omnetpp::simsignal_t sentDFSignal;

    /**
     * @brief Simsignal for received remote frames.
     */
    omnetpp::simsignal_t sentRFSignal;

    /**
     * @brief Current drift of the can clock.
     */
    double currentDrift;

    /**
     * @brief Collection including all
     */
    std::list<CanDataFrame*> outgoingDataFrames;

private:
    /**
     * @brief Overhead for a data frame.
     *
     * The variable includes Start of frame, arbitration field (11 Bit), remote transmission bit,
     * identifier extension bit, reserved bit, data length code, CRC, CRC-delimiter, ACK-slot,
     * ACK-delimiter, end of frame and inter frame space.
     */
    static const unsigned int DATAFRAMECONTROLBITS = 47;

    /**
     * @brief Additional bit needed for the 29 bit ID.
     */
    static const unsigned int ARBITRATIONFIELD29BIT = 20;

    /**
     * @brief Number of control bits which are used in bit-stuffing. Just 34 of the 47 are subject to bit-stuffing.
     */
    static const unsigned int CONTROLBITSFORBITSTUFFING = 34;

    /**
     * @brief Maximum ID value for Version 2.0A.
     */
    static const unsigned int VERSIONAMAX = 2047;

    /**
     * @brief Maximum ID value for Version 2.0B.
     */
    static const unsigned int VERSIONBMAX = 536870911;

    /**
     * @brief The version of CAN used in this network. Valid values: 2.0A or 2.0B
     */
    std::string canVersion;

    /**
     * @brief Value for the percentage distribution for bit stuffing. Valid values: 0 to 1.
     */
    double bitStuffingPercentage;

    /**
     * @brief Creates a data frame which will be queued in the buffer.
     */
    void initialRemoteFrameCreation();

    /**
     * @brief Registers the outgoing remote frame at the port.
     *
     * @param canID the ID of the remote frame
     */
    void registerRemoteFrameAtPort(unsigned int canID);

    /**
     * @brief Creates a data frame which will be queued in the buffer.
     */
    void initialDataFrameCreation();

    /**
     * @brief Registers the outgoing data frames at the port to receive incoming remote frames.
     *
     * @param canID the ID of the data frame
     */
    void registerDataFrameAtPort(unsigned int canID);

    /**
     * @brief Checks whether the CAN-ID matches the restrictions of the CAN version.
     *
     * @param canID the ID that will be checked
     *
     * @return Returns the ID unchanged.
     */
    unsigned int checkAndReturnID(unsigned int canID);

    /**
     * @brief Calculates the additional bits.
     *
     * For the calculation the parameter #bitStuffingPercentage is used. A value of 0 means no bit stuffing while a value of 100 stands for the worst case.
     *
     * @param dataLength Size of the data field in bytes
     * @param arbFieldLength Size of the arbitration field
     *
     * @return Returns the number of stuffing bits.
     */
    unsigned int calculateStuffingBits(unsigned int dataLength, unsigned int arbFieldLength);

    /**
     * @brief Transmits a data or remote frame to the connected output buffer.
     *
     * @param df the frame that should be sent
     */
    void frameTransmission(CanDataFrame *df);
};
}
#endif /* CANTRAFFICSOURCEAPPBASE_H_ */
