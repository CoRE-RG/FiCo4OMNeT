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

package busmodel.bus.can;

simple CanBusApp
{
    parameters:
    	@display("i=block/app");
    	
        @signal[receivedDF](type=CanDataFrame);
        @signal[receivedEF](type=ErrorFrame);
        
        @statistic[receivedDF](title="data frames received"; source=receivedDF; record=count; interpolationmode=none);
        @statistic[receivedEF](title="error frames received"; source=receivedEF; record=count; interpolationmode=none);
    gates:
        input frameIn @directIn;
}