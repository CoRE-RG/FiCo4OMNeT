//Copyright (c) 2014, CoRE Research Group, Hamburg University of Applied Sciences
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification,
//are permitted provided that the following conditions are met:
//
//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
//3. Neither the name of the copyright holder nor the names of its contributors
//   may be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
//ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

package fico4omnet.applications.can.source;

import fico4omnet.applications.ISourceApplication;

//
// This module generates all data and remote frames for the node. Periodically sent messages
// are scheduled and forwarded to the output buffer at the given time. A frame with a period 
// of 0 is only forwarded when a remote frame from another node is received.
// 
// @see ~ISourceApplication
//
// @author Stefan Buschmann
//
simple CanTrafficSourceAppBase like ISourceApplication
{
    parameters:
        @class(CanTrafficSourceAppBase);
        @display("i=block/source");
             
        //Signal for transmitted data frames.
        @signal[txDF](type=CanDataFrame);
        //Signal for transmitted remote frames.
        @signal[txRF](type=CanDataFrame);
        
        //Statistic of the number of transmitted data frames.
        @statistic[txDF](title="source app data frame"; source=txDF; record=count);
        //Statistic of the number of transmitted remote frames.
        @statistic[txRF](title="source app remote frame"; source=txRF; record=count);
        
        //The Data Frames ID(s) - String parameter (int) separated with commas
        string idDataFrames = default("0");					
        //data Frame period(s) for sending of messages - String parameter (int) separated with commas - unit: ms 
        string periodicityDataFrames = default("0");		
        //Datafield-length - String parameter (int) separated with commas - unit: byte
        string dataLengthDataFrames = default("0");			
        //Offset for the first transmission of the data frames - String parameter (double) separated with commas - unit: s
        string initialDataFrameOffset = default("0");		
        //The Remote Frame ID(s) - String parameter (int) separated with commas
        string idRemoteFrames = default("0");				
        //Remote Frame period(s) for sending of messages - String parameter (int) separated with commas - unit: ms
        string periodicityRemoteFrames = default("0"); 		
        //Datafield-length - String parameter (int) separated with commas - unit: byte
        string dataLengthRemoteFrames = default("0");		
        //Offset for the first transmission of the remote frames - String parameter (double) separated with commas - unit: s
        string initialRemoteFrameOffset = default("0");		
        //Inaccuracy for the node which is added to the schedule time for each frame. 
        double periodInaccurracy @unit(s) = default(0s);
        
    gates:
        //Frames are forwarded via this output gate to the next module. 
        output out @labels(DataFrame);
        //Received remote frames are received at this gate. 
        input remoteIn @directIn;
}