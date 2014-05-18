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

package fico4omnet.bus.can;


import fico4omnet.bus.BusPort;
import fico4omnet.bus.can.CanBusLogic;

// Central unit of the CAN-Network
//
// Forwards data-frames and error-frames to all participating nodes of the network
//
module CanBus
{
    parameters:
        bool errorsActivated = default(false);		// activate if errors shall be enabled
        double bandwidth @unit(bps) = default(1Mbps); //[MBit/s]
        string version @enum("2.0A", "2.0B") = default("2.0A");					// Version of CAN-Protocol. 2.0A = 11 Bits identifier, 2.0B = 29 Bits identifier
        double bitStuffingPercentage = default(0);			// Value for the percentage distribution for bit stuffing. Valid values 0 to 1.
        @display("i=block/network");
    
    gates:
        inout gate[];

    submodules:
        busPort: BusPort {
            @display("p=163,74");
            gates:
                phygate[sizeof(gate)];
        }
        canBusLogic: CanBusLogic {
            @display("p=52,74");                
        }

    connections allowunconnected:
        busPort.innerGate <--> canBusLogic.gate;
        for i=0..sizeof(gate)-1 {
            busPort.phygate[i] <--> gate[i];
        }
}
