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

#ifndef __FICO4OMNET_CANPORTOUTPUT_H_
#define __FICO4OMNET_CANPORTOUTPUT_H_

#include <omnetpp.h>
#include "ErrorFrame_m.h"
#include "CanDataFrame_m.h"

namespace FiCo4OMNeT {

/**
 * @brief Outgoing messages are handled in this module.
 *
 * @ingroup Port
 *
 * @author Stefan Buschmann
 */
class CanPortOutput: public cSimpleModule {
public:
    virtual ~CanPortOutput();

    /**
     * @brief Handles received error frames so that scheduled error events can be canceled.
     */
    virtual void handleReceivedErrorFrame();

    /**
     * @brief Is called when the transmission of a frame is completed.
     */
    virtual void sendingCompleted();
protected:
    /**
     * @brief Initialization of the module.
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
     * @brief Maximum size of an error frame.
     */
    static const int MAXERRORFRAMESIZE = 12;

    /**
     * @brief Simsignal for sent data frames.
     */
    simsignal_t sentDFSignal;

    /**
     * @brief Simsignal for sent remote frames.
     */
    simsignal_t sentRFSignal;

    /**
     * @brief Simsignal for receive errors.
     */
    simsignal_t receiveErrorsSignal;

    /**
     * @brief Simsignal for send errors.
     */
    simsignal_t sendErrorsSignal;

    /**
     * @brief Bandwidth of the bus in Mbps.
     */
    double bandwidth;

    /**
     * true if errors are activated. Initialized from ned-attribute of CAN-Node
     */
    bool errorsActivated;

    /**
     * @brief Probability that an error for the received frame will occur.
     *
     */
    int errorperc;

    /**
     * @brief The currently scheduled error frame.
     */
    ErrorFrame *scheduledErrorFrame;

    /**
     * @brief True if an error frame for the current data frame is already received.
     */
    bool errorReceived;

    /**
     * @brief Initializes the values needed for the stats collection.
     */
    virtual void initializeStatisticValues();

    /**
     * @brief Calculates when the frame is ready to be forwarded based on the number of bits.
     */
    virtual double calculateScheduleTiming(int length);

    /**
     * @brief Colors the connection to the bus to represent it is busy.
     */
    virtual void colorBusy();

    /**
     * @brief Colors the connection to the bus to represent it is idle.
     */
    virtual void colorIdle();

    /**
     * @brief Colors the connections to the bus to represent a transmission of an error frame.
     */
    virtual void colorError();

};

}

#endif /* CANPORTOUTPUT_H_ */
