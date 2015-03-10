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


package fico4omnet.buffer.can;

import fico4omnet.buffer.can.ICanBuffer;

//
// Incoming frames are forwarded from the port to the input buffer. As soon as a frame is received
// a message is sent to the sink application. The sink application afterwards requests a frame and
// the buffer delivers it to the application. 
//
simple CanInputBuffer like ICanBuffer
{
    parameters:
        @display("i=block/buffer");
        //Statistic of the number of packets that were sent by the buffer
        @signal[rxPk](type=cMessage);
        @statistic[rxPk](title="RX Packets"; source=rxPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        //Statistic of the amount of packets that were sent by the buffer
        @signal[txPk](type=cMessage);
        @statistic[txPk](title="TX Packets"; source=txPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);

        //Comma seperated list of gates where the frames of the buffer are delivered
        string destination_gates = default("sinkApp[0].dataIn");
        bool MOB = default(true); 						//If true frames with the same ID will be overwritten.
        string idIncomingFrames = default("0");			// The Incoming Frame ID(s) - String parameter (int) separated with commas

    gates:
        //The buffers Input
        input in[] @loose @labels(DataFrame);
        input directIn @directIn;
        //The buffers Output
        output out @loose @labels(DataFrame);
}
