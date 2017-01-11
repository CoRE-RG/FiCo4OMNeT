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

import fico4omnet.buffer.flexray.IFRBuffer;

//
// @todo Documentation
//
// @see ~IFRBuffer
//
// @author Stefan Buschmann
//
simple FRInputBuffer like IFRBuffer
{
    parameters:
        @display("i=block/buffer");
        
        //Statistic of the amount of packets that were sent by the buffer
        //@statistic[txPk](title="TX Packets"; record=count,vector);
        //End-to-end latency
        //@statistic[latency](title="End-to-end latency"; unit=s; record=stats,histogram,vector);
        //Statistic of the amount of packets that were sent by the buffer
        @signal[rxPk](type=cMessage);
        @statistic[rxPk](title="RX Packets"; source=rxPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        //Statistic of the amount of packets that were sent by the buffer
        @signal[txPk](type=cMessage);
        @statistic[txPk](title="TX Packets"; source=txPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        //Signal emitted when queuelength changes, contains new length
        @signal[length](type=unsigned long; unit=packets);
        //Signal emitted when queuelength changes, contains new size in bytes
        @signal[size](type=unsigned long; unit=B); 
        //Statistic of the queue length of the buffer
        @statistic[length](title="Queue Length"; source=length; unit=packets; record=vector,stats; interpolationmode=sample-hold);
        //Statistic of the queue size of the buffer in bytes
        @statistic[size](title="Queue Size"; source=size; record=vector,stats; unit=B; interpolationmode=sample-hold);   

        //Comma seperated list of gates where the frames of the buffer are delivered
        string destination_gates = default("");
        bool MOB = default(true); //If true frames with the same ID will be overwritten.
        
    gates:
        //The buffers Input
        input in[] @loose @labels(DataFrame);
        input directIn @directIn;
        //The buffers Output @warning CURRENTLY NOT USED!
        output out @loose @labels(DataFrame);
}
