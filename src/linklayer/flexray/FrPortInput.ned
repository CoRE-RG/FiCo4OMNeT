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

simple FRPortInput
{
//    parameters:
//        @signal[receivedCompleteDF](type=CanDataFrame);
//        @signal[receivedCompleteRF](type=CanDataFrame);
//        
//        @statistic[receivedCompleteDF](title="complete data frames received"; source=receivedCompleteDF; record=count; interpolationmode=none);
//        @statistic[receivedCompleteRF](title="complete remote frames received"; source=receivedCompleteRF; record=count; interpolationmode=none);
        
    gates:
        output out @loose;
        input inChA @loose;
        input inChB @loose;
}
