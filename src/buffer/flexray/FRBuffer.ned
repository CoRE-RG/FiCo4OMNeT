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

package fico4omnet.buffer.flexray;

import fico4omnet.buffer.IBuffer;

//
// @todo Documentation
//
// @see ~IBuffer
//
// @author Stefan Buschmann
//
moduleinterface IFRBuffer extends IBuffer
{
    parameters:
        @display("i=block/buffer");
        
        //Statistic of the amount of packets that were sent by the buffer
        //@statistic[txPk](title="TX Packets"; record=count,vector);
        //End-to-end latency
        //@statistic[latency](title="End-to-end latency"; unit=s; record=stats,histogram,vector);

        bool MOB;//If true frames with the same ID will be overwritten.
}
