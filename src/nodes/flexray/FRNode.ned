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

package busmodel.nodes.flexray;

import busmodel.scheduler.flexray.FRScheduler;
import busmodel.applications.flexray.FRApp;
import busmodel.applications.flexray.source.FRTrafficSourceAppBase;
import busmodel.applications.flexray.sink.FRTrafficSinkAppBase;
import busmodel.buffer.flexray.FROutputBuffer;
import busmodel.buffer.flexray.FRInputBuffer;
import busmodel.linklayer.flexray.FRNodePort;
import busmodel.synchronisation.flexray.FRSync;

module FRNode
{
    parameters:
        int pChannels = default(2); //0 = A; 1 = B; 2 = A&B 
        @display("bgb=353,272;i=device/device");
        @node;

    gates:
        inout channelA @loose;
        inout channelB @loose;

    submodules:

        frNodePort: FRNodePort {
            @display("p=155,180");
        }

        frScheduler: FRScheduler {
            @display("p=304,109");
        }

        frApp: FRApp {
            @display("p=33,31");
        }

        frSourceApp: FRTrafficSourceAppBase {
            @display("p=102,31");
        }

        frSinkApp: FRTrafficSinkAppBase {
            @display("p=212,31");
        }

        frOutputBuffer: FROutputBuffer {
            @display("p=102,109");
        }

        frInputBuffer: FRInputBuffer {
            @display("p=212,109");
        }

        frSync: FRSync {
            @display("p=304,31");
        }

    connections allowunconnected:
        frSourceApp.out --> frOutputBuffer.in;
        frSinkApp.in <-- frInputBuffer.out;

        frNodePort.upperLayerOut --> frInputBuffer.in;
        frNodePort.upperLayerIn <-- frOutputBuffer.out;

        channelA <--> frNodePort.phyChA if pChannels == 0 || pChannels == 2;
        channelB <--> frNodePort.phyChB if pChannels == 1 || pChannels == 2;
}
