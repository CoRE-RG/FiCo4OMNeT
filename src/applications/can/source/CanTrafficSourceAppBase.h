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

#ifndef CANTRAFFICSOURCEAPPBASE_H_
#define CANTRAFFICSOURCEAPPBASE_H_

#include <omnetpp.h>
#include <string.h>
#include "CanBuffer.h"
#include "CanPortInput.h"
#include "candataframe_m.h"

using namespace std;

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
    string canVersion;

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
    vector<CanDataFrame*> outgoingDataFrames;

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
Define_Module(CanTrafficSourceAppBase)
;
#endif /* CANTRAFFICSOURCEAPPBASE_H_ */
