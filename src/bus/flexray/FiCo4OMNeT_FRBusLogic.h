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

#ifndef __FICO4OMNET_FRBUSLOGIC_H_
#define __FICO4OMNET_FRBUSLOGIC_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <limits>
#include "FiCo4OMNeT_BusPort.h"
#include "SchedulerMessage_m.h"

namespace FiCo4OMNeT {

/**
 * @brief Represents the logic of the bus. It provides several statistic values.
 *
 *
 * @ingroup Bus
 *
 * @author Stefan Buschmann
 */
class FRBusLogic: public cSimpleModule {

protected:
    /**
     * @brief collects data for the  results
     *
     * Calculates the busload, number of messages and errors.
     *
     */
    virtual void finish();

    /**
     * @brief Initializes all participants of the network
     *
     * All participants will be delivered with a message containing their vector-ID.
     * This is used to guarantee the generic behaviour of the simulation.
     *
     */
    virtual void initialize();

    /**
     * @brief Is called when a new message is received
     */
    virtual void handleMessage(cMessage *msg);

private:
    /**
     * @brief Signal for received data frames.
     */
    simsignal_t rcvdStaticFrameSignal;

    /**
     * @brief Signal for received remote frames.
     */
    simsignal_t rcvdDynamicFrameSignal;
    /**
     * @brief Colors the connections of the bus to represent it is busy.
     */
    virtual void colorBusy();
    /**
     * @brief Colors the connections of the bus to represent it is idle.
     */
    virtual void colorIdle();
    /**
     * @brief Colors the connections of the bus to represent a transmission of an error frame.
     */
    virtual void colorError();
};

}

#endif /* FRBUSAPP_H_ */
