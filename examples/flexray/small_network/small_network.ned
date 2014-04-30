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

package fico4omnet.examples.flexray.small_network;

import fico4omnet.nodes.flexray.FRNode;
import fico4omnet.bus.flexray.FRBus;

//
// Small sample network with three hosts and one switch that shows how to 
// configure the TTEthernet Model. Configuration in omnetpp.ini
// - Uses 3ms Cycletime
// - 80ns Tick length
//
network small_network
{
    @display("bgb=334,259");
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 10ns;
        }
    submodules:
        bus: FRBus {
            parameters:
                @display("p=159,122;b=269,24,rect");
        }

        unit1: FRNode {
            @display("p=68,38");
        }

        unit2: FRNode {
            @display("p=244,38");
        }

        unit3: FRNode {
            @display("p=68,194");
        }

        unit4: FRNode {
            @display("p=160,194");
        }

        unit5: FRNode {
            @display("p=244,194");
        }
    connections:
        unit1.channelA <--> Channel <--> bus.channelA[0];
        unit1.channelB <--> Channel <--> bus.channelB[0];
        unit2.channelB <--> Channel <--> bus.channelB[1];
        unit3.channelA <--> Channel <--> bus.channelA[1];
        unit3.channelB <--> Channel <--> bus.channelB[2];
        unit4.channelA <--> Channel <--> bus.channelA[2];
        unit4.channelB <--> Channel <--> bus.channelB[3];
        unit5.channelA <--> Channel <--> bus.channelA[3];

}
