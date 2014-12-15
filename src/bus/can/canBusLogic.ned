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
//
simple CanBusLogic
{
    parameters:
    	@display("i=block/app");
    	
        @signal[receivedDF](type=CanDataFrame);
        @signal[receivedRF](type=CanDataFrame);
        @signal[receivedEF](type=ErrorFrame);
        @signal[state];
        
        @statistic[receivedDF](title="data frames received"; source=receivedDF; record=count; interpolationmode=none);
        @statistic[ID-DF](title="IDs data frames received"; source="ID(receivedDF)"; record=histogram?,vector; interpolationmode=none);
        @statistic[receivedRF](title="remote frames received"; source=receivedRF; record=count; interpolationmode=none);
        @statistic[ID-RF](title="IDs remote frames received"; source="ID(receivedRF)"; record=histogram?,vector?; interpolationmode=none);
        @statistic[receivedEF](title="error frames received"; source=receivedEF; record=count; interpolationmode=none);
        @statistic[ID-EF](title="IDs error frames received"; source="ID(receivedEF)"; record=histogram?,vector?; interpolationmode=none);
        @statistic[bitPerSec](title="Bit/sec"; unit="bps"; source="(packetBits(receivedDF)+packetBits(receivedRF))"; record=last);
        //@statistic[framesPerSec](title="Frames/sec"; unit="fps"; source="sumPerDuration(removeNaN(constant1(receivedDF)))"; record=last);
        @statistic[utilization](title="Utilization"; unit="%"; source="lowHighRatio(state)*100"; record=last);
        @statistic[test](title="Test"; unit=""; source="count(removeNaN(packetBits(receivedEF)))"; record=last);
        //@statistic[errorsPercent](title="Errors %"; unit="%"; source="removeNaN(count(receivedEF))/((removeNaN(count(receivedDF))+removeNaN(count(receivedRF)))*100)"; record=last);
    gates:
        inout gate;
}