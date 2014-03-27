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

package busmodel.linklayer.can;

import busmodel.linklayer.can.CanPortInput;
import busmodel.linklayer.can.CanPortOutput;
import busmodel.linklayer.NodePort;

//
// This module establishes the communication with the bus and the buffers. 
//
// It consists of the submodules NodePort, CanPortOutput and CanPortInput.
//
//
module CanNodePort
{
    @display("bgb=199,193");
    gates:
        inout phygate;
        //Input from the upper layer
        input upperLayerIn @loose;
        //Output to the upper layer
        output upperLayerOut @loose;

    submodules:
        canPortInput: CanPortInput {
            @display("p=132,51");
        }
        canPortOutput: CanPortOutput {
            @display("p=54,51");
        }
        nodePort: NodePort {
            @display("p=93,119");
        }

    connections:
        nodePort.phygate <--> phygate;
        nodePort.upperLayerOut --> canPortInput.in;
        nodePort.upperLayerIn <-- canPortOutput.out;
        upperLayerIn --> canPortOutput.in;
        upperLayerOut <-- canPortInput.out;

}
