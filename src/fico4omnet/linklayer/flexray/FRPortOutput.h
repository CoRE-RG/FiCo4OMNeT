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

#ifndef __FICO4OMNET_FRPORTOUTPUT_H_
#define __FICO4OMNET_FRPORTOUTPUT_H_

//FiCo4OMNeT
#include "fico4omnet/base/Defs.h"

namespace FiCo4OMNeT {

// TODO Documentation
class FRPortOutput: public cSimpleModule {

public:
    /**
     * @brief Constructor
     */
    FRPortOutput();

    /**
     * @brief Is called when the transmission of a frame is completed.
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
//    /**
//     * @brief Simsignal for sent frames in static segment.
//     */
//    simsignal_t sentStaticChASignal;
//
//    /**
//     * @brief Simsignal for sent frames in dynamic segment.
//     */
//    simsignal_t sentDynamicChASignal;
//
//    /**
//     * @brief Simsignal for sent frames in static segment.
//     */
//    simsignal_t sentStaticChBSignal;
//
//    /**
//     * @brief Simsignal for sent frames in dynamic segment.
//     */
//    simsignal_t sentDynamicChBSignal;

    /**
     * @brief Valid values are between 10000 and 1000000. Initialized from ned-attribute of CAN-Bus.
     */
    int bandwidth;

//    /**
//     * @brief Initializes the values needed for the stats collection.
//     */
//    virtual void initializeStatisticValues();

    /**
     * @brief Colors the connection to the bus to represent it is busy.
     */
    virtual void colorBusy();

    /**
     * @brief Colors the connection to the bus to represent it is idle.
     */
    virtual void colorIdle();
};

}
#endif /* FRPORTOUTPUT_H_ */
