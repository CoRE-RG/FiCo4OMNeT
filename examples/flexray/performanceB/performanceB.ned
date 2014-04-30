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

package fico4omnet.examples.flexray.performanceB;

import fico4omnet.nodes.flexray.FRNode;
import fico4omnet.bus.flexray.FRBus;

//
// Small sample network with three hosts and one switch that shows how to 
// configure the TTEthernet Model. Configuration in omnetpp.ini
// - Uses 3ms Cycletime
// - 80ns Tick length
//
network performanceB
{
    @display("bgb=687,259");
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 10ns;
        }
    submodules:
        bus: FRBus {
            parameters:
                @display("p=315,122;b=580,24,rect");
        }

        unit1: FRNode {
            @display("p=91,38");
        }

        unit2: FRNode {
            @display("p=153,38");
        }

        unit3: FRNode {
            @display("p=215,38");
        }

        unit4: FRNode {
            @display("p=276,38");
        }

        unit5: FRNode {
            @display("p=354,38");
        }

        unit6: FRNode {
            @display("p=415,38");
        }

        unit7: FRNode {
            @display("p=479,38");
        }

        unit8: FRNode {
            @display("p=546,38");
        }

        unit9: FRNode {
            @display("p=91,192");
        }

        unit10: FRNode {
            @display("p=153,192");
        }

        unit11: FRNode {
            @display("p=215,192");
        }

        unit12: FRNode {
            @display("p=276,192");
        }

        unit13: FRNode {
            @display("p=354,192");
        }

        unit14: FRNode {
            @display("p=415,192");
        }

        unit15: FRNode {
            @display("p=479,192");
        }

        unit16: FRNode {
            @display("p=546,192");
        }
        
        unit17: FRNode {
            @display("p=546,192");
        }
        
        unit18: FRNode {
            @display("p=546,192");
        }
        
        unit19: FRNode {
            @display("p=546,192");
        }

        unit20: FRNode {
            @display("p=546,192");
        }

        
        connections:
        unit1.channelA <--> Channel <--> bus.channelA[0];
        unit2.channelA <--> Channel <--> bus.channelA[1];
        unit3.channelA <--> Channel <--> bus.channelA[2];
        unit4.channelA <--> Channel <--> bus.channelA[3];
        unit5.channelA <--> Channel <--> bus.channelA[4];
        unit6.channelA <--> Channel <--> bus.channelA[5];
        unit7.channelA <--> Channel <--> bus.channelA[6];
        unit8.channelA <--> Channel <--> bus.channelA[7];
        unit9.channelA <--> Channel <--> bus.channelA[8];
        unit10.channelA <--> Channel <--> bus.channelA[9];
        unit11.channelA <--> Channel <--> bus.channelA[10];
        unit12.channelA <--> Channel <--> bus.channelA[11];
        unit13.channelA <--> Channel <--> bus.channelA[12];
        unit14.channelA <--> Channel <--> bus.channelA[13];
        unit15.channelA <--> Channel <--> bus.channelA[14];
        unit16.channelA <--> Channel <--> bus.channelA[15];
        unit17.channelA <--> Channel <--> bus.channelA[16];
        unit18.channelA <--> Channel <--> bus.channelA[17];
        unit19.channelA <--> Channel <--> bus.channelA[18];
        unit20.channelA <--> Channel <--> bus.channelA[19];


}
