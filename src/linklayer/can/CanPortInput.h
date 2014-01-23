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

#ifndef CANPORTINPUT_H_
#define CANPORTINPUT_H_

#include <omnetpp.h>
#include <string.h>
#include "candataframe_m.h"
#include "err_m.h"

using namespace std;

class CanPortInput : public cSimpleModule{
protected:
    /**
     *
     */
    virtual void initialize();

    /**
     * @brief Handles all received messages
     *
     * @param msg the incoming message.
     */
    virtual void handleMessage(cMessage *msg);

private:
    /**
     * @brief Vector with IDs of relevant incoming data frames
     */
    vector<int> incomingDataFrameIDs;

    /**
     * @brief Vector with IDs of relevant incoming remote frames
     */
    vector<int> incomingRemoteFrameIDs;

    /**
     * @brief Valid values are between 10000 and 1000000. Initialized from ned-attribute of CAN-Bus.
     */
    int bandwidth;

    /**
     * true if errors are activated. Initialized from ned-attribute of CAN-Node.
     */
    bool errors;

    /**
    * if errors = true then this is the amount of errors appearing on the node in percent. Initialized from ned-attribute of CAN-Node
    *
    */
    int errorperc;

    /**
     * @brief Incoming Data frame is scheduled until receiving is completed.
     */
    virtual void receiveMessage(CanDataFrame *msg);

    /**
     * @brief
     */
    virtual void handleError(CanDataFrame *df);

    /**
     * @brief Checks whether the received frame is relevant for this node.
     */
    virtual bool checkExistence(CanDataFrame *df);

    /**
     * @brief Calculates when the frame is ready to be forwarded based on the number of bits.
     */
    virtual double calculateScheduleTiming(int length);

    /**
     * @brief Sends the message to the output gate if incomingIDs contains the message ID or to the SourceApp in case of a remote frame.
     */
    virtual void forwardDataFrame(CanDataFrame *msg);

    /**
     * @brief Sends the error frame to the output gate
     */
    virtual void forwardErrorFrame(ErrorFrame *ef);
};
Define_Module(CanPortInput);
#endif /* CANPORTINPUT_H_ */
