package busmodel.examples.can.error1;

import busmodel.bus.can.CanBus;
import busmodel.nodes.can.CanNode;

// Generic network configuration for CAN-Networks
// 
// Can be used for various number of CAN-Nodes, but the design of the network can't be changed. Used for all examples.
network Error1
{
	@display("bgb=550,350,white");
    submodules:
        bus: CanBus {
            gates:
                gate[2];
        }
        node[2]: CanNode;
    connections:
//        for i=0..(bus.nodecount-1) {
//            bus.gate++ <--> node[i].gate;
//        }

        bus.gate[0] <--> node[0].gate;
        bus.gate[1] <--> node[1].gate;
}