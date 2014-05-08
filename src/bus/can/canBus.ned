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
        string version = default("2.0A");					// Version of CAN-Protocol. 2.0A = 11 Bits identifier, 2.0B = 29 Bits identifier
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
