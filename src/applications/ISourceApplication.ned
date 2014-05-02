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

package fico4omnet.applications;

// 
// Interface for a source application
//
moduleinterface ISourceApplication
{
    parameters:
        @display("i=block/source");
//        string idDataFrames;					// The Data Frames ID(s) - String parameter (int) separated with commas
//        string periodicityDataFrames;			// data Frame period(s) for sending of messages - String parameter (int) separated with commas
//        string dataLengthDataFrames;			// Datafield-length - String parameter (int) separated with commas
//        string idRemoteFrames;					// The Remote Frame ID(s) - String parameter (int) separated with commas
//        string periodicityRemoteFrames; 		// Remote Frame period(s) for sending of messages - String parameter (int) separated with commas
//        string dataLengthRemoteFrames;			// Datafield-length - String parameter (int) separated with commas
    gates:
        output out;
}