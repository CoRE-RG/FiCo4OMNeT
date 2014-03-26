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

//
// This submodule is part of the node port module. Received frames are processed here. This includes the 
// forwarding to the input buffer, the transmission of remote frames to the source app and the generation
// of receive errors.
//
//
simple CanPortInput
{
    parameters:
        @signal[receivedCompleteDF](type=CanDataFrame);
        @signal[receivedCompleteRF](type=CanDataFrame);
        
        @statistic[receivedCompleteDF](title="complete data frames received"; source=receivedCompleteDF; record=count; interpolationmode=none);
        @statistic[receivedCompleteRF](title="complete remote frames received"; source=receivedCompleteRF; record=count; interpolationmode=none);
        
    gates:
        output out @loose;
        input in @loose;
}
