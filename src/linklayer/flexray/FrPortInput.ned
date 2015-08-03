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

//
// @todo Documentation
//
// @author Stefan Buschmann
//
simple FRPortInput
{
    parameters:
        @signal[receivedCompleteSF](type=FRFrame);
        @signal[receivedCompleteDF](type=FRFrame);
        
        @statistic[receivedCompleteSF](title="complete static frames received"; source=receivedCompleteSF; record=count; interpolationmode=none);
        @statistic[receivedCompleteDF](title="complete dynamic frames received"; source=receivedCompleteDF; record=count; interpolationmode=none);
        
    gates:
        output out @loose;
        input inChA @loose;
        input inChB @loose;
}
