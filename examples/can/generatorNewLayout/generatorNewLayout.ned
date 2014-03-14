package busmodel.simulations.can.generatorNewLayout;

import busmodel.bus.can.CanBus;
import busmodel.nodes.can.CanNode;

// Generic network configuration for CAN-Networks
// 
// Can be used for various number of CAN-Nodes, but the design of the network can't be changed. Used for all examples.
network generatorNewLayout
{
    @display("bgb=931,343,white");
    submodules:
        bus: CanBus {
            @display("p=460,127");
        }
        aws: CanNode {
            @display("p=257,47;i=device/pc2");
        }
        motorx: CanNode {
            @display("p=66,227;i=block/cogwheel");
        }
        motory: CanNode {
            @display("p=205,266;i=block/cogwheel");
        }
        motorz: CanNode {
            @display("p=350,216;i=block/cogwheel");
        }
        motoralpha: CanNode {
            @display("p=493,266;i=block/cogwheel");
        }
        motorbeta: CanNode {
            @display("p=649,241;i=block/cogwheel");
        }
        motorgamma: CanNode {
            @display("p=829,227;i=block/cogwheel");
        }
        generator: CanNode {

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
