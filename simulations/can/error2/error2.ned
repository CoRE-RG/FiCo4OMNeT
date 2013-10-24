package busmodel.simulations.can.error2;

import busmodel.bus.can.canBus;
import busmodel.nodes.can.canNode;

// Generic network configuration for CAN-Networks
// 
// Can be used for various number of CAN-Nodes, but the design of the network can't be changed. Used for all examples.
network Canbus
{
	@display("bgb=550,350,white");
    submodules:
        bus: canBus {
            @display("i=abstract/bus");
        }
        node[bus.nodecount]: canNode;
    connections:
        for i=0..(bus.nodecount-1) {
            bus.gate++ <--> node[i].gate;
        }
}