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

package fico4omnet.nodes.can;

import fico4omnet.linklayer.can.CanNodePort;
import fico4omnet.buffer.can.CanInputBuffer;
import fico4omnet.buffer.can.CanOutputBuffer;
import fico4omnet.applications.ISourceApplication;
import fico4omnet.applications.can.source.CanTrafficSourceAppBase;
import fico4omnet.applications.can.sink.CanTrafficSinkAppBase;

//
// Participants in the network. 
//
// Generates data, remote and error frames.
//
//
module CanNode
{
    parameters:
        int numSourceApps = default(1);
        string version = default("2.0A");					// Version of CAN-Protocol. 2.0A = 11 Bits identifier, 2.0B = 29 Bits identifier
        double bitStuffingPercentage = default(0);			// Value for the percentage distribution for bit stuffing. Valid values 0 to 1.
        int errorperc = default(0);							// If errors are enabled: percentage of errors
        int numOfMessages = default(1);						// Number of messages of the node
//        string idDataFrames = default("0");					// The Data Frames ID(s) - String parameter (int) separated with commas
//        string periodicityDataFrames = default("0");		// data Frame period(s) for sending of messages - String parameter (int) separated with commas
//        string dataLengthDataFrames = default("0");			// Datafield-length - String parameter (int) separated with commas
//        string idRemoteFrames = default("0");				// The Remote Frame ID(s) - String parameter (int) separated with commas
//        string periodicityRemoteFrames = default("0"); 		// Remote Frame period(s) for sending of messages - String parameter (int) separated with commas
//        string dataLengthRemoteFrames = default("0");		// Datafield-length - String parameter (int) separated with commas
        string idIncomingFrames = default("0");				// The Incoming Frame ID(s) - String parameter (int) separated with commas
        string node = default("0");							// Name of the node
        @display("i=block/app2;bgb=248,222");

    gates:
        inout gate;

    submodules:
        canNodePort: CanNodePort {
            @display("p=149,168");
            gates:
                phygate;
        }
        bufferIn: CanInputBuffer {
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
//        sourceApp: CanTrafficSourceAppBase {
//            @display("p=98,32");
//        }
        sinkApp: CanTrafficSinkAppBase {
            @display("p=203,32");
        }
    connections:
        canNodePort.phygate <--> gate;
        canNodePort.upperLayerIn <-- bufferOut.out;
        canNodePort.upperLayerOut --> bufferIn.in[0];
        sinkApp.in <-- bufferIn.out;
//        sourceApp.out --> bufferOut.in;
		sourceApp[0].out --> bufferOut.in[0];
//        for i=0..sizeof(numSourceApps)-1 {
//            sourceApp[i].out --> bufferOut.in[i];
//        }
}
