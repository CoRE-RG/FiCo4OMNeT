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

package busmodel.applications.can.source;

import busmodel.applications.ISourceApplication;
//
// This module generates all data and remote frames for the node. Periodically sent messages
// are scheduled and forwarded to the output buffer at the given time. A frame with a period 
// of 0 is only forwarded when a remote frame from another node is received. All relevant parameters
// for the frames are defined in the ini file. 
// 
//
//
simple CanTrafficSourceAppBase like ISourceApplication
{
    parameters:
        @class(CanTrafficSourceAppBase);
        @display("i=block/source");
    gates:
        output out @labels(DataFrame);
        input remoteIn @directIn;
}