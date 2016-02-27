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

package fico4omnet.scheduler.can;

//
// This module simulates the clock drift of a can node. 
//
// @author Stefan Buschmann
//
simple CanClock
{
    parameters:
    	@display("i=block/cogwheel");
    	
    	//Signal for the current drift of the clock
    	@signal[clockDrift];
    	
    	//Statistic about the drift of the clock
    	@statistic[clockDrift](title="clockDrift"; source=clockDrift; unit=s; record=stats,histogram?,vector; interpolationmode=linear);
    	
    	//Maximum drift per second
    	double maxDrift @unit(s) = default(0);
    	//Maximum drift change per second
    	double maxDriftChange @unit(s) = default(0);
    	//True if the node should have a random drift at the start of the simulation, false otherwise.
    	bool randomStartDrift = default(true);
}