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

package fico4omnet.linklayer.flexray;

//
// @todo Documentation
//
// @author Stefan Buschmann
//
simple FRPortOutput
{
//    parameters:
//        @signal[sentDF](type=CanDataFrame);
//        @signal[sentRF](type=CanDataFrame);
//        @signal[receiveError](type=ErrorFrame);
//        @signal[sendError](type=ErrorFrame);
//
//        @statistic[sentDF](title="sent data frames"; source=sentDF; record=count; interpolationmode=none);
//        @statistic[sentRF](title="sent remote frames"; source=sentRF; record=count; interpolationmode=none);
//        @statistic[receiveError](title="receive errors"; source=receiveError; record=count; interpolationmode=none);
//        @statistic[sendError](title="sent errors"; source=sendError; record=count; interpolationmode=none);

    gates:
        output outChA @loose;
        output outChB @loose;
        input in @loose;
//        input directIn @directIn;
}