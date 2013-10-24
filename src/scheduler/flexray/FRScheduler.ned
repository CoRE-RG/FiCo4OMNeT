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

package busmodel.scheduler.flexray;


simple FRScheduler
{
    parameters:
    	@display("i=block/cogwheel");
    	         
        int gCycleCountMax = default(63); // [7,9,...,63]
        double pdMicrotick @unit(s) = default(12.5ns); //[ns]
        double gdMacrotick @unit(s) = default(1us); //[µs]
        int gdStaticSlot = default(4); //[MT]
        int gdMinislot = default(2); //[MT]
        int gdNIT = default(2); //[MT]
        int gdSymbolWindow = default(0); //[MT]
        int gNumberOfMinislots = default(0);
        int gNumberOfStaticSlots = default(2);
        int gdActionPointOffset = default(1); //[MT]
        int gdMinislotActionPointOffset = default(1); //[MT]
        double busSpeed @unit(Mbps) = default(10Mbps); //[MBit/s]
        
        double maxDriftChange @unit(s) = default(0);
        double maxDrift @unit(s) = default(0);
        
        string staticSlotsChA = default(""); 
        string staticSlotsChB = default("");
        string dynamicSlotsChA = default("");
        string dynamicSlotsChB = default("");
        int syncFrame = default(0);
        
        double currentTick @unit(s) = default(0);
}