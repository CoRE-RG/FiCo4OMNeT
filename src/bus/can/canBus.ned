package busmodel.bus.can;


import busmodel.bus.BusPort;
import busmodel.bus.can.CanBusLogic;

// Central unit of the CAN-Network
//
// Forwards data-frames and error-frames to all participating nodes of the network
//
module CanBus
{
    parameters:
        bool errorsActivated = default(false);		// activate if errors shall be enabled
        double bandwidth @unit(bps) = default(1Mbps); //[MBit/s]
//        int nodecount = default(2); 		// number of nodes in the network
        @display("i=block/network");
    
    gates:
//        inout gate[nodecount];
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
