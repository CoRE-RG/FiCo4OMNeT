package can_fr.bus.can;

// Central unit of the CAN-Network
//
// Forwards data-frames and error-frames to all participating nodes of the network
//

import can_fr.bus.busPort;
import can_fr.bus.can.canBusApp;

module CAN_Bus
{
    parameters:
        bool payload = default(false);		// activate if payload transfer shall be enabled
        bool errors = default(false);		// activate if errors shall be enabled
        bool ack = default(false);			// activate if ack-recognition of receiving nodes shall be enabled
        int bandwidth = default(1000000);	// valid values are 10000 - 1000000
        int nodecount = default(2); 		// number of nodes in the network
        @display("i=block/network");
    gates:
        inout gate[nodecount];

    submodules:
        canBusApp: canBusApp {
            @display("p=52,74");
        }
        bus: busPort {
            @display("p=163,74");
            gates:
                gate[nodecount];
        }


    connections:
        for i=0..nodecount-1 {
            bus.gate[i] <--> gate[i];
        }
}
