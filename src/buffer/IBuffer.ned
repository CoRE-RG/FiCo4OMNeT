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

package fico4omnet.buffer;

//
// Interface representing a buffer module.
//
// @author Stefan Buschmann
//
moduleinterface IBuffer
{
    parameters:
        @display("i=block/buffer");
        
        //Signal for transmitted frames.
        @signal[rxPk](type=cMessage);
        //Statistic of the number of frames that were sent by the buffer
        @statistic[rxPk](title="RX Packets"; source=rxPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
		//Signal for received frames.
        @signal[txPk](type=cMessage);
        //Statistic of the amount of frames that were sent by the buffer
        @statistic[txPk](title="TX Packets"; source=txPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);

        //Comma seperated list of gates where the frames of the buffer are delivered
        string destination_gates;
        
    gates:
        //The buffers Input
        input in[] @loose;
        //Direct input for the buffer
        input directIn @directIn;
        //The buffers Output @warning CURRENTLY NOT USED!
        output out @loose;
}
