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

package fico4omnet.applications.can.sink;

import fico4omnet.applications.ISinkApplication;

//
// Received data frames are processed in this module. When the input buffer receives a frame, 
// the application gets informed about this. As soon as the application is idle it requests the next frame
// from the buffer.
// 
//
simple CanTrafficSinkAppBase like ISinkApplication
{
    parameters:
        @class(CanTrafficSinkAppBase);
        @display("i=block/sink");
        string idIncomingFrames = default("0");				// The Incoming Frame ID(s) - String parameter (int) separated with commas
    gates:
        input in @labels(DataFrame);
        input controllerIn @directIn;
}