package busmodel.examples.can.arbitration;

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
            gates:
                gate[5];
        }
        node[5]: CanNode;
    connections:
//        for i=0..(bus.nodecount-1) {
//            bus.gate++ <--> node[i].gate;
//        }
		bus.gate[0] <--> node[0].gate;
		bus.gate[1] <--> node[1].gate;
		bus.gate[2] <--> node[2].gate;
		bus.gate[3] <--> node[3].gate;
		bus.gate[4] <--> node[4].gate;
}
