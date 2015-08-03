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

package fico4omnet.bus.flexray;

import fico4omnet.bus.BusPort;
import fico4omnet.bus.flexray.FRBusLogic;

//
// @todo Documentation
//
// @author Stefan Buschmann
//
module FRBus
{
    parameters:
        int numberOfNodesChannelA = default(0);
        int numberOfNodesChannelB = default(0);

    gates:
        inout channelA[numberOfNodesChannelA];
        inout channelB[numberOfNodesChannelB];

    submodules:
        busPortChannelA: BusPort {
            @display("p=68,138");
            gates:
                phygate[numberOfNodesChannelA];
        }
        
        busPortChannelB: BusPort {
            @display("p=163,138");
            gates:
                phygate[numberOfNodesChannelB];
        }
        
        frBusLogic: FRBusLogic {
            @display("p=116,46");
        }

    connections:
        for i=0..numberOfNodesChannelA-1 {
            busPortChannelA.phygate[i] <--> channelA[i];
        }
        for i=0..numberOfNodesChannelB-1 {
            busPortChannelB.phygate[i] <--> channelB[i];
        }

        frBusLogic.gateChA <--> busPortChannelA.innerGate;
        frBusLogic.gateChB <--> busPortChannelB.innerGate;
}
