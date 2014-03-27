package busmodel.examples.can.payload;

import busmodel.bus.can.CanBus;
import busmodel.nodes.can.CanNode;

// Generic network configuration for CAN-Networks
// 
// Can be used for various number of CAN-Nodes, but the design of the network can't be changed. Used for all examples.
network Canbus
{
	@display("bgb=550,350,white");
    submodules:
        bus: CanBus {
            @display("i=abstract/bus");
        }
        node[bus.nodecount]: CanNode;
    connections:
        for i=0..(bus.nodecount-1) {
            bus.gate++ <--> node[i].gate;
        }
}