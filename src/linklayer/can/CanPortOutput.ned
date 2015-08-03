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
// @author Stefan Buschmann
//
simple CanPortOutput
{
    parameters:
        //Signal for transmitted data frames
        @signal[txDF](type=CanDataFrame);
        //Signal for transmitted remote frames
        @signal[txRF](type=CanDataFrame);
        //Signal for transmitted error frames
        @signal[txEF](type=ErrorFrame);
        //Signal for received error frames
        @signal[rxEF](type=ErrorFrame);

		//Statistic about the number of transmitted data frames.
        @statistic[txDF](title="sent data frames"; source=txDF; record=count);
        //Statistic about the number of transmitted remote frames.
        @statistic[txRF](title="sent remote frames"; source=txRF; record=count);
        //Statistic about the number of transmitted error frames.
        @statistic[txEF](title="sent errors"; source=txEF; record=count);
        //Statistic about the number of received error frames.
        @statistic[rxEF](title="receive errors"; source=rxEF; record=count);

    gates:
        output out @loose;
        input in @loose;
        input directIn @directIn;
}
