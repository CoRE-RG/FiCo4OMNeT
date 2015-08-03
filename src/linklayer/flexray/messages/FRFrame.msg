namespace FiCo4OMNeT;

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

//namespace std;

packet FRFrame {
    // Set FlexRay Protocol Pitlength
    byteLength = 8;
    int frameID;
    int cycleNumber;
    int payloadLength; //The payload segment size is encoded in this field by setting it to the number of payload data bytes divided by two
    int channel;
    bool syncFrameIndicator;
}

//
// Enum for all possible channel combinations 
//
// @author Stefan Buschmann
enum Channel {//0 = A; 1 = B; 2 = A&B
    // Event that signals the beginning of a new cycle
    CHANNEL_A=0;
    // Event with a specified action time
    CHANNEL_B=1;
    // Event that is triggered after a specified amount of time
    CHANNEL_AB=2;
}