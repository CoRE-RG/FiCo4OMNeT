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

package can_fr.bus.flexray;

import can_fr.bus.flexray.FRTopologyPort;

module FRBus
{
    parameters:
        int numberOfNodesChannelA = default(0);
        int numberOfNodesChannelB = default(0);

    gates:
        inout channelA[numberOfNodesChannelA];
        inout channelB[numberOfNodesChannelB];
        
    submodules:
        frTopologyPort: FRTopologyPort {
            @display("p=402,59");
            gates:
                phyChannelA[numberOfNodesChannelA];
                phyChannelB[numberOfNodesChannelB];
        }

    connections:
        for i=0..numberOfNodesChannelA-1 {
            frTopologyPort.phyChannelA[i] <--> channelA[i];
        }
        for i=0..numberOfNodesChannelB-1 {
            frTopologyPort.phyChannelB[i] <--> channelB[i];
        }

}
