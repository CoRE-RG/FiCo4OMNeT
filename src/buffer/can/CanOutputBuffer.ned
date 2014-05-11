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

package fico4omnet.buffer.can;

import fico4omnet.buffer.can.CanBuffer;

simple CanOutputBuffer like CanBuffer
{
        parameters:
        @display("i=block/buffer");
        //Comma seperated list of gates where the frames of the buffer are delivered
        string destination_gates = default("");
        bool MOB = default(true);//If true frames with the same ID will be overwritten.
    gates:
        //The buffers Input
        input in[] @loose @labels(DataFrame);
        //The buffers Output @warning CURRENTLY NOT USED!
        output out @loose @labels(DataFrame);

}
