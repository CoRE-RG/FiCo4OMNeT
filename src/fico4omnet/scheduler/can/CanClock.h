//Copyright (c) 2014, CoRE Research Group, Hamburg University of Applied Sciences
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification,
//are permitted provided that the following conditions are met:
//
//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
//3. Neither the name of the copyright holder nor the names of its contributors
//   may be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
//ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef FICO4OMNET_CANCLOCK_H_
#define FICO4OMNET_CANCLOCK_H_

//FiCo4OMNeT
#include "fico4omnet/base/FiCo4OMNeT_Defs.h"

namespace FiCo4OMNeT {

using namespace omnetpp;

/**
 * @brief The CanClock simulates the clock drift of a can node.
 *
 * The drift of the clock of a can node is calculated based on the maximum drift per second, the maximum drift change per second and the time that passed since the last drift update.
 *
 * @author Stefan Buschmann
 */
class CanClock : public cSimpleModule {
public:
    /**
     * @brief Constructor
     */
    CanClock();

    /**
     * @brief getter for the current drift
     *
     * @return returns the current drift of the clock
     */
    double getCurrentDrift();

private:
    /**
     * @brief Signal to emit the current drift.
     */
    simsignal_t clockDriftSignal;

    /**
     * @brief contains the current drift
     */
    double currentDrift;

    /**
     * @brief The drift of the clock can not exceed this value (positive and negative).
     */
    double maxDrift;

    /**
     * @brief Within one second the drift of the clock can not change for more than this value (positive and negative).
     */
    double maxDriftChange;
	
	/**
	 * @brief True if the initial clock drift should be random. False otherwise.
	 */    
    bool randomStartDrift;

    /**
     * @brief This holds the time when the last drift change was applied.
     */
    simtime_t lastDriftUpdate;

    /**
     * @brief Calculates the new drift based on #currentDrift, #maxDrift, #maxDriftChange and #lastDriftUpdate
     */
    void calculateNewDrift();

    /**
     * @brief If an initial Drift is activated in the ini, a random drift is calculated.
     */
    void calculateInitialDrift();

protected:
    /**
     * @brief Initialization of the module.
     */
    virtual void initialize();


};

}
#endif /* FICO4OMNET_CANCLOCK_H_ */
