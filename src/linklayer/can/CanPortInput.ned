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

package fico4omnet.linklayer.can;

//
// This submodule is part of the node port module. Received frames are processed here. This includes the 
// forwarding to the input buffer, the transmission of remote frames to the source app and the generation
// of receive errors.
//
simple CanPortInput
{
    parameters:
        //Signal for received data frames
        @signal[rxDF](type=CanDataFrame);
        //Signal for received remote frames
        @signal[rxRF](type=CanDataFrame);
        //Signal for received payload within a received data frame.
        @signal[rxDFPayload](type=cPacket);
        //Signal for received payload within a received remote frame.
        @signal[rexRFPayload](type=cPacket);
        
        //Statistic about the number of received data frames.
        @statistic[rxDF](title="complete data frames received"; source=rxDF; record=count);
        //Statistic about the number of received remote frames.
        @statistic[rxRF](title="complete remote frames received"; source=rxRF; record=count);
        //Statistic about the end to end latency of the received data frames.
        @statistic[rxDFLatency](title="End-to-end latency of data frames"; source="timestampAge(rxDF)"; unit=s; record=stats,histogram?,vector; interpolationmode=linear);
        //Statistic about the end to end latency of the received remote frames.
        @statistic[rxRFLatency](title="End-to-end latency of remote frames"; source="timestampAge(rxRF)"; unit=s; record=stats,histogram?,vector; interpolationmode=linear);
        //Statistic about the end to end latency of the payload within the received data frame. 
        @statistic[rxDFPayloadLatency](title="End-to-end latency of data frame payload"; source="timestampAge(rxDFPayload)"; unit=s; record=stats,histogram?,vector; interpolationmode=linear);
        //Statistic about the end to end latency of the payload within the received remote frame. 
        @statistic[rxRFPayloadLatency](title="End-to-end latency of remote frame payload"; source="timestampAge(rxRFPayload)"; unit=s; record=stats,histogram?,vector; interpolationmode=linear);
    gates:
        output out @loose;
        input in @loose;
}
