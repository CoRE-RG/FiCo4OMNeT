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

package fico4omnet.nodes.can;

import fico4omnet.linklayer.can.CanNodePort;
import fico4omnet.buffer.can.CanInputBuffer;
import fico4omnet.buffer.can.CanOutputBuffer;
import fico4omnet.applications.ISourceApplication;
import fico4omnet.applications.ISinkApplication;
import fico4omnet.applications.can.source.CanTrafficSourceAppBase;
import fico4omnet.applications.can.sink.CanTrafficSinkAppBase;

//
//
// The node consists of several submodules which have different tasks. The source applications generate data and remote frames,
// the sink applications process incoming frames, the buffers save the data and remote frames while the portmodule is responsible
// for receiving and sending data and remote frames.
//
// It is possible to generate any number of source applications which forward their frames all to the same buffer.
//
//
//

module CanNode
{
    parameters:
        int numSourceApps = default(1);						// Number of source apps in this node
        int numSinkApps = default(1);						// Number of source apps in this node
        int numInputBuffer = default(1);					// Number of input buffer in this node
        int errorperc = default(0);							// If errors are enabled: percentage of errors
        @display("i=block/app2;bgb=248,222");

    gates:
        inout gate;

    submodules:
        canNodePort: CanNodePort {
            @display("p=149,168");
            gates:
                phygate;
        }
        bufferIn[numInputBuffer]: CanInputBuffer {
            @display("p=203,102");
            gates:
                in[1];
        }
        bufferOut: CanOutputBuffer {
            @display("p=98,102");
            gates:
                in[numSourceApps];
        }
        sourceApp[numSourceApps]: <default("CanTrafficSourceAppBase")> like ISourceApplication {
            @display("p=98,32,row,70");
        }
        sinkApp[numSinkApps]: <default("CanTrafficSinkAppBase")> like ISinkApplication {
            @display("p=203,32");
        }
    connections:
        canNodePort.phygate <--> gate;
        canNodePort.upperLayerIn <-- bufferOut.out;
//        canNodePort.upperLayerOut --> bufferIn.in[0];
//		for i=0..numInputBuffer-1 {
//            canNodePort.upperLayerOut[i] <-- bufferIn.out[i];
//        }
        for i=0..numSourceApps-1 {
            sourceApp[i].out --> bufferOut.in[i];
        }
}
