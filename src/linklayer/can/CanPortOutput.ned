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
// This submodule is part of the node port module. Outgoing frames are processed here. This includes the 
// transmission of data, remote and error frames as well as the generation of send error frames.
//
//
simple CanPortOutput
{
    parameters:
        @signal[sentDF](type=CanDataFrame);
        @signal[sentRF](type=CanDataFrame);
        @signal[receiveError](type=ErrorFrame);
        @signal[sendError](type=ErrorFrame);

        @statistic[sentDF](title="sent data frames"; source=sentDF; record=count; interpolationmode=none);
        @statistic[sentRF](title="sent remote frames"; source=sentRF; record=count; interpolationmode=none);
        @statistic[receiveError](title="receive errors"; source=receiveError; record=count; interpolationmode=none);
        @statistic[sendError](title="sent errors"; source=sendError; record=count; interpolationmode=none);

    gates:
        output out @loose;
        input in @loose;
        input directIn @directIn;
}
