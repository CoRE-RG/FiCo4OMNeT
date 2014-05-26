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

#ifndef __FICO4OMNET_FRAPP_H_
#define __FICO4OMNET_FRAPP_H_

#include <iostream>
#include "FRFrame_m.h"
#include "flexray/SchedulerMessage_m.h"
#include "flexray/SchedulerMessageEvents_m.h"
#include "FiCo4OMNeT_SchedulerEvent.h"
#include "FiCo4OMNeT_FRScheduler.h"
#include "FiCo4OMNeT_FRPort.h"

namespace FiCo4OMNeT {

class FRApp : public cSimpleModule {

    public:
    /**
     * @brief Will be set by the FRScheduler so it can take a maximum of 1/4 of the dynamic segment.
     */
        virtual void setMaxRandom(int max);

	protected:
        /**
         * @brief Initialization of the module.
         */
        virtual void initialize();

        /**
         * @brief Handles all scheduler messages
         *
         * @param msg the incoming message.
         */
        virtual void handleMessage(cMessage *msg);

        /**
         * @brief Creates FlexRay-message
         */
        virtual FRFrame* createFRFrame(int frameID, int cycleNumber, int channel, bool syncFrameIndicator, int kind);

	private:
        int maxRandom;
        SimTime lastcreate;
        virtual int randomSize();
};

}
#endif /* FRAPP_H_ */
