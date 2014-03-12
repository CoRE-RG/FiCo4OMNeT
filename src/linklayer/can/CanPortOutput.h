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

#ifndef CANPORTOUTPUT_H_
#define CANPORTOUTPUT_H_

#include <omnetpp.h>
#include "err_m.h"
#include "candataframe_m.h"

class CanPortOutput: public cSimpleModule {
public:
    /**
     * @brief Handles received error frames so that scheduled error events can be canceled.
     */
    virtual void handleReceivedErrorFrame();

    /**
     *
     */
    virtual void sendingCompleted();
protected:
    /**
     * @brief Initialization of the module.
     */
    virtual void initialize();

    virtual void finish();

    /**
     * @brief Handles all received messages
     *
     * @param msg the incoming message.
     */
    virtual void handleMessage(cMessage *msg);

private:

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
     * @brief Valid values are between 10000 and 1000000. Initialized from ned-attribute of CAN-Bus.
     */
    int bandwidth;

    /**
     * true if errors are activated. Initialized from ned-attribute of CAN-Node
     */
    bool errors;

    /**
     * if errors = true then this is the amount of errors appearing on the node in percent. Initialized from ned-attribute of CAN-Node
     *
     */
    int errorperc;

    /**
     * @brief The currently scheduled error frame.
     */
    ErrorFrame *scheduledErrorFrame;

    bool errorReceived;

    /**
     * @brief Initializes the values needed for the stats collection.
     */
    virtual void initializeStatisticValues();

    /**
     * @brief Generates the values for the statistics
     */
    virtual void collectStats();

    /**
     * @brief Calculates when the frame is ready to be forwarded based on the number of bits.
     */
    virtual double calculateScheduleTiming(int length);

    /**
     *
     */
    virtual void colorBusy();

    /**
     *
     */
    virtual void colorIdle();

    /**
     *
     */
    virtual void colorError();

};
Define_Module(CanPortOutput);
#endif /* CANPORTOUTPUT_H_ */
