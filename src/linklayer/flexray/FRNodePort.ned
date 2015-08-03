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

package fico4omnet.linklayer.flexray;

import fico4omnet.linklayer.flexray.FRPortInput;
import fico4omnet.linklayer.flexray.FRPortOutput;
import fico4omnet.linklayer.NodePort;

//
// @todo Documentation
//
// @author Stefan Buschmann
//
module FRNodePort
{
    @display("bgb=199,193");

    gates:
        inout phyChA @loose;
        inout phyChB @loose;
        //Input from the upper layer
        input upperLayerIn @loose;
        //Output to the upper layer
        output upperLayerOut @loose;

    submodules:
        frPortOutput: FRPortOutput {
            @display("p=54,51");
        }
        
        frPortInput: FRPortInput {
            @display("p=132,51");
        }
        
        nodePortChA: NodePort {
            @display("p=54,138");
        }
        
        nodePortChB: NodePort {
            @display("p=132,138");
        }

    connections:
        nodePortChA.phygate <--> phyChA;
        nodePortChB.phygate <--> phyChB;
        nodePortChA.upperLayerOut --> frPortInput.inChA;
        nodePortChA.upperLayerIn <-- frPortOutput.outChA;
        nodePortChB.upperLayerOut --> frPortInput.inChB;
        nodePortChB.upperLayerIn <-- frPortOutput.outChB;
        upperLayerIn --> frPortOutput.in;
        upperLayerOut <-- frPortInput.out;
}
