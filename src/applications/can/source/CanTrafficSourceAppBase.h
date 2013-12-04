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
#include "Buffer.h"
#include "candataframe_m.h"

using namespace std;

//class Buffer;

class CanTrafficSourceAppBase: public cSimpleModule {

protected:
    /**
     * @brief Initialization of the module.
     */
    virtual void initialize();

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
    static const int DATAFRAMEOVERHEAD = 47;

    /**
     * @brief Additional bit needed for the 29 bit ID.
     */
    static const int ARBITRATIONFIELD29BIT = 20;

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
     *
     */
    int currentFrameID;

    /**
     * @brief Collection including all
     */
    vector<CanDataFrame*> outgoingRemoteFrames;

    /**
     * @brief Creates a data frame which will be queued in the buffer.
     */
    void initialRemoteFrameCreation();

    /**
     * @brief Collection including all
     */
    vector<CanDataFrame*> outgoingDataFrames;

    /**
     * @brief Creates a data frame which will be queued in the buffer.
     */
    void initialDataFrameCreation();

    /**
     *
     */
    int checkAndReturnID(int id);

    /**
     * @brief Calculates the length for the data frame.
     */
    int calculateLength(int datalength);

    /**
     * @brief Transmits the data frame to the connected output buffer.
     */
    void dataFrameTransmission(CanDataFrame *df);
};
Define_Module(CanTrafficSourceAppBase);
#endif /* CANTRAFFICSOURCEAPPBASE_H_ */
