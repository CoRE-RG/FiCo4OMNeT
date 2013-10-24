package busmodel.simulations.can.generator;

import busmodel.bus.can.canBus;
import busmodel.nodes.can.canNode;

// Generic network configuration for CAN-Networks
// 
// Can be used for various number of CAN-Nodes, but the design of the network can't be changed. Used for all examples.
network generator_CAN
{
    @display("bgb=931,343,white;i=bus/philips");
    submodules:
        bus: canBus {
            @display("p=460,127;i=abstract/bus");
        }
        aws: canNode {
            @display("p=257,47;i=device/pc2");
        }
        motorx: canNode {
            @display("p=66,227;i=block/cogwheel");
        }
        motory: canNode {
            @display("p=205,266;i=block/cogwheel");
        }
        motorz: canNode {
            @display("p=350,216;i=block/cogwheel");
        }
        motoralpha: canNode {
            @display("p=493,266;i=block/cogwheel");
        }
        motorbeta: canNode {
            @display("p=649,241;i=block/cogwheel");
        }
        motorgamma: canNode {
            @display("p=829,227;i=block/cogwheel");
        }
        generator: canNode {

            @display("p=539,47;i=block/control");
        }
    connections:
        bus.gate++ <--> aws.gate;
        bus.gate++ <--> motorx.gate;
        bus.gate++ <--> motory.gate;
        bus.gate++ <--> motorz.gate;
        bus.gate++ <--> motoralpha.gate;
        bus.gate++ <--> motorbeta.gate;
        bus.gate++ <--> motorgamma.gate;
        bus.gate++ <--> generator.gate;
}
