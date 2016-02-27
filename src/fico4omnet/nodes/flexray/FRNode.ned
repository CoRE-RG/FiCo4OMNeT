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

package fico4omnet.nodes.flexray;

import fico4omnet.scheduler.flexray.FRScheduler;
import fico4omnet.applications.flexray.source.FRTrafficSourceAppBase;
import fico4omnet.applications.flexray.sink.FRTrafficSinkAppBase;
import fico4omnet.buffer.flexray.FROutputBuffer;
import fico4omnet.buffer.flexray.FRInputBuffer;
import fico4omnet.linklayer.flexray.FRNodePort;
import fico4omnet.synchronisation.flexray.FRSync;

//
// @todo Documentation
//
// @author Stefan Buschmann
//
module FRNode
{
    parameters:
        @display("bgb=331,272;i=device/device");
        
        int pChannels = default(2); //0 = A; 1 = B; 2 = A&B        
        int gPayloadLengthStatic = default(0);
        int cPayloadLengthMax = default(0);
        int gCycleCountMax = default(63); // [7,9,...,63]
        double pdMicrotick @unit(s) = default(12.5ns); //[ns]
        double gdMacrotick @unit(s) = default(1us); //[�s]
        int gdStaticSlot = default(4); //[MT]
        int gdMinislot = default(2); //[MT]
        int gdNIT = default(2); //[MT]
        int gdSymbolWindow = default(0); //[MT]
        int gNumberOfMinislots = default(0);
        int gNumberOfStaticSlots = default(2);
        int gdActionPointOffset = default(1); //[MT]
        int gdMinislotActionPointOffset = default(1); //[MT]
        double bandwidth @unit(bps) = default(10000000bps); //[MBit/s]
        double maxDriftChange @unit(s) = default(0);
        double maxDrift @unit(s) = default(0);

//        @node;

    gates:
        inout channelA @loose;
        inout channelB @loose;

    submodules:
        frNodePort: FRNodePort {
            @display("p=128,183");
        }

        frScheduler: FRScheduler {
            @display("p=277,112");
        }

        frSourceApp: FRTrafficSourceAppBase {
            @display("p=75,34");
        }

        frSinkApp: FRTrafficSinkAppBase {
            @display("p=185,34");
        }

        outputBuffer: FROutputBuffer {
            @display("p=75,112");
            gates:
                in[1];
        }

        inputBuffer: FRInputBuffer {
            @display("p=185,112");
            gates:
                in[1];
        }

        frSync: FRSync {
            @display("p=277,34");
        }

    connections allowunconnected:
        frSourceApp.out --> outputBuffer.in[0];
        frSinkApp.in <-- inputBuffer.out;

        frNodePort.upperLayerOut --> inputBuffer.in[0];
        frNodePort.upperLayerIn <-- outputBuffer.out;

        channelA <--> frNodePort.phyChA if pChannels == 0 || pChannels == 2;
        channelB <--> frNodePort.phyChB if pChannels == 1 || pChannels == 2;
}
