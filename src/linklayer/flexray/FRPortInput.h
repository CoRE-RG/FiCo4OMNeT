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

#ifndef FRPORTINPUT_H_
#define FRPORTINPUT_H_

#include <omnetpp.h>
#include <string.h>
#include "FRFrame_m.h"
#include "FRScheduler.h"
#include "FRSync.h"

using namespace std;

/**
 * @brief Received messages are initially handled in this module.
 *
 * @ingroup Port
 *
 * @author Stefan Buschmann
 */
class FRPortInput: public cSimpleModule{
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
     * @brief Valid values are between 10000 and 1000000. Initialized from ned-attribute of CAN-Bus.
     */
    int bandwidth;

    /**
     * @brief Handles the reception of a dynamic frame.
     */
    virtual void receiveDynamicFrame(FRFrame *frMsg);

    /**
     * @brief Handles the reception of a static frame.
     */
    virtual void receiveStaticFrame(FRFrame *frMsg);

    /**
     * @brief Incoming frame is scheduled until receiving is completed.
     */
    virtual void receiveMessage(FRFrame *msg);

    /**
     * @brief Calculates when the frame is ready to be forwarded based on the number of bits.
     */
    virtual double calculateScheduleTiming(int length);

    /**
     * @brief Sends the message to the output gate.
     */
    virtual void forwardFrame(FRFrame *msg);
};

#endif /* FRPORTINPUT_H_ */
