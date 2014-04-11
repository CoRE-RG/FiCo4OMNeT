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

#ifndef FRTRAFFICSOURCEAPPBASE_H_
#define FRTRAFFICSOURCEAPPBASE_H_

#include <omnetpp.h>
#include <string.h>
#include <clistener.h>
#include <SchedulerMessage_m.h>
#include "FRBuffer.h"
#include "FRFrame_m.h"

using namespace std;

/**
 * @brief Traffic source application used to generate outgoing messages.
 *
 * @ingroup Applications
 *
 * @author Stefan Buschmann
 */
class FRTrafficSourceAppBase: public cSimpleModule, cListener {

public:

    virtual void receiveSignal(cComponent *source, simsignal_t signalID, long l);


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
     * @brief Collection including all outgoing static frames.
     */
    vector<FRFrame*> outgoingStaticFrames;

    /**
     * @brief Collection including all outgoing dynamic frames.
     */
    vector<FRFrame*> outgoingDynamicFrames;

    /**
     * @brief Number of the current cycle.
     */
    int vCycleCounter;

    /**
     * @brief Initial creation of all the nodes static frames.
     */
    void setUpStaticFrames();

    /**
     * @brief Initial creation of all the nodes dynamic frames.
     */
    void setUpDynamicFrames();

    /**
     *
     */
    void dynamicFrameCreation(cStringTokenizer tokenizer, int channel);

    /**
     *
     */
    FRFrame* createFRFrame(int frameID, int cycleNumber, int channel,
            bool syncFrameIndicator, int kind);

    int getDynamicSlot(int slot);

    /**
     * @brief Calculates the length for the data frame.
     */
    int calculateLength(int datalength);

    /**
     * @brief Transmits the frame to the connected output buffer.
     */
    void frameGenerationForNewCycle();

    void transmitFrame(FRFrame *frMsg);
};
Define_Module(FRTrafficSourceAppBase)
;
#endif /* FRTRAFFICSOURCEAPPBASE_H_ */
