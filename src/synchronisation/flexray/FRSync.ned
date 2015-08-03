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

package fico4omnet.synchronisation.flexray;

//
// Dummy synchronisation module that corrects the clock with a certain imprecission.
// It uses the simulation time as basis.
//
// @see ~IPort
//
// @author Till Steinbach
//
simple FRSync
{
    parameters:
        @display("i=block/timer");
        
        int pOffsetCorrectionOut = default(15);
		int pRateCorrectionOut = default(3);
		int pClusterDriftDamping = default(0);
}
