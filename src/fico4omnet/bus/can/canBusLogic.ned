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

package fico4omnet.bus.can;

//
// The arbitration is handeled in this module. If a node has a message it registers the Can ID at the CanBusLogic.
// The bus logic grants the sending permission to the node with the lowest Can ID. Is the frame transmission 
// completed, the next permission is again granted to the node with the highest priority. 
//
// @author Stefan Buschmann
//
simple CanBusLogic
{
    parameters:
    	@display("i=block/app");
    	
    	//Signal for all received frames
        @signal[rxPk](type=CanDataFrame);
        //Signal for all received data frames
        @signal[rxDF](type=CanDataFrame);
        //Signal for all received remote frames
        @signal[rxRF](type=CanDataFrame);
        //Signal for all received error frames
        @signal[rxEF](type=ErrorFrame);
        //Signal for the state of the bus - idle or busy
        @signal[state];
        
        //Statistic about the number of received data frames
        @statistic[rxDF](title="data frames received"; source=rxDF; record=count; interpolationmode=none);
        //Statistic about the can IDs received via data frames
        @statistic[rxCANIdDF](title="received CAN IDs via DF"; source="ID(rxDF)"; record=histogram?,vector?; interpolationmode=none);
        //Statistic about the number of received remote frames
        @statistic[rxRF](title="remote frames received"; source=rxRF; record=count; interpolationmode=none);
        //Statistic about the can IDs received via remote frames 
        @statistic[rxCANIdRF](title="received CAN IDs via RF"; source="ID(rxRF)"; record=histogram?,vector?; interpolationmode=none);
        //Statistic about the number of received error frames
        @statistic[rxEF](title="error frames received"; source=rxEF; record=count; interpolationmode=none);
        //Statistic about the can IDs received that generated an error
        @statistic[rxCANIdEF](title="received CAN IDs via EF"; source="ID(rxEF)"; record=histogram?,vector?; interpolationmode=none);
		//Statistic about the frames per second over the whole simulation time
		@statistic[framesPerSec](title="Frames/sec"; unit="fps"; source="sumPerDuration(constant1(rxPk))"; record=last);
		//Statistic about the bits per second over the whole simulation time
        @statistic[bitsPerSec](title="Bits/sec"; unit="bps"; source="sumPerDuration(packetBits(rxPk))"; record=last);
        //Statistic about the percentage utilization of the bus. 
        @statistic[utilization](title="Utilization"; unit="%"; source="lowHighRatio(state)*100"; record=last);
        
    gates:
        inout gate;
}