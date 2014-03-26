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

package busmodel.nodes.can;

import busmodel.linklayer.can.CanNodePort;
import busmodel.buffer.can.InputBuffer;
import busmodel.buffer.can.OutputBuffer;
import busmodel.applications.can.source.CanTrafficSourceAppBase;
import busmodel.applications.can.sink.CanTrafficSinkAppBase;

//
// Participants in the network. 
//
// Generates data, remote and error frames.
//
//
//
module CanNode
{
    parameters:
        bool extcode = default(false);						// Activate if external code shall be used (has to be deactivated in this version)
        bool payload = default(false);						// Activate if payload shall be enabled
        string version = default("2.0A");					// Version of CAN-Protocol. 2.0A = 11 Bits identifier, 2.0B = 29 Bits identifier
        int bitStuffingPercentage = default(50);			// For Bit-Stuffing mode 2
        int bitStuffingMethod = default(0);					// Bit-Stuffing mode. See .ini files for further information
        bool errors = default(false);						// Activate if errors shall be enabled (must be activated for CAN_Bus and CAN_Node)
        int errorperc = default(10);						// If errors are enabled: percentage of errors
        bool ack = default(false);							// Activate if ack-recognition of receiving nodes shall be enabled (must be activated for CAN_Bus and CAN_Node)
        int bandwidth = default(1000000);					// valid values are 10000 - 1000000 (must be same for CAN_Bus and CAN_Node)
        int numOfMessages = default(1);						// Number of messages of the node
        int numOfMobs = default(2);							// Number of physical message buffers
        bool trafficGen = default(true);					// Activate if traffic-generator shall be used (must be activated in this version)	
        string data = default("0");							// The data - String parameter (string) separated with commas
        string idDataFrames = default("0");					// The Data Frames ID(s) - String parameter (int) separated with commas
        string periodicityDataFrames = default("0");		// data Frame period(s) for sending of messages - String parameter (int) separated with commas
        string dataLengthDataFrames = default("0");			// Datafield-length - String parameter (int) separated with commas
        string idRemoteFrames = default("0");				// The Remote Frame ID(s) - String parameter (int) separated with commas
        string periodicityRemoteFrames = default("0"); 		// Remote Frame period(s) for sending of messages - String parameter (int) separated with commas
        string dataLengthRemoteFrames = default("0");		// Datafield-length - String parameter (int) separated with commas
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
        bufferIn: InputBuffer {
            @display("p=203,102");
        }
        bufferOut: OutputBuffer {
            @display("p=98,102");
        }
        sourceApp: CanTrafficSourceAppBase {
            @display("p=98,32");
        }
        sinkApp: CanTrafficSinkAppBase {
            @display("p=203,32");
        }
    connections:
        canNodePort.phygate <--> gate;
        canNodePort.upperLayerIn <-- bufferOut.out;
        canNodePort.upperLayerOut --> bufferIn.in;
        sourceApp.out --> bufferOut.in;
        sinkApp.in <-- bufferIn.out;

}
