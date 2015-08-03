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

//
// The arbitration is handeled in this module. If a node has a message it registers the Can ID at the CanBusLogic.
// The bus logic grants the sending permission to the node with the lowest Can ID. Is the frame transmission 
// completed, the next permission is again granted to the node with the highest priority. 
//
// @author Stefan Buschmann
//
simple FRBusLogic
{
    parameters:
    	@display("i=block/app");
    	
    	@signal[receivedStaticFrame](type=FRFrame);
        @signal[receivedDynamicFrame](type=FRFrame);
        
        @statistic[receivedStaticFrame](title="static frames received"; source=receivedStaticFrame; record=count; interpolationmode=none);
        @statistic[ID-StaticFrame](title="IDs static frames received"; source="ID(receivedStaticFrame)"; record=histogram?,vector?; interpolationmode=none);
        @statistic[receivedDynamicFrame](title="dynamic frames received"; source=receivedDynamicFrame; record=count; interpolationmode=none);
        @statistic[ID-DynamicFrame](title="IDs dynamic frames received"; source="ID(receivedDynamicFrame)"; record=histogram?,vector?; interpolationmode=none);

    gates:
        inout gateChA;
        inout gateChB;
}