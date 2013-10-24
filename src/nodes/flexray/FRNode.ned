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
import busmodel.linklayer.flexray.FRPort;
import busmodel.synchronisation.flexray.FRSync;

module FRNode
{
    parameters:
        int pChannels = default(2); //0 = A; 1 = B; 2 = A&B 
        @display("bgb=299,224;i=device/device");
        @node;

    gates:
        inout channelA @loose;
        inout channelB @loose;

    submodules:

        frPort: FRPort {
            @display("p=155,155");
        }

        frScheduler: FRScheduler {
            @display("p=155,90");
        }

        frApp: FRApp {
            @display("p=75,51");
        }

        frSync: FRSync {
            @display("p=235,51");
        }

    connections allowunconnected:
        channelA <--> frPort.phyChannelA if pChannels == 0 || pChannels == 2;
        channelB <--> frPort.phyChannelB if pChannels == 1 || pChannels == 2;
}
