package fico4omnet.examples.can.generatorMulti;

import fico4omnet.bus.can.CanBus;
import fico4omnet.nodes.can.CanNode;

// Generic network configuration for CAN-Networks
// 
// Can be used for various number of CAN-Nodes, but the design of the network can't be changed. Used for all examples.
network generatorMulti
{
    @display("bgb=931,343,white");
    submodules:
        bus: CanBus {
            @display("p=460,127");
            gates:
                gate[8];
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
        bus.gate[0] <--> aws.gate;
        bus.gate[1] <--> motorx.gate;
        bus.gate[2] <--> motory.gate;
        bus.gate[3] <--> motorz.gate;
        bus.gate[4] <--> motoralpha.gate;
        bus.gate[5] <--> motorbeta.gate;
        bus.gate[6] <--> motorgamma.gate;
        bus.gate[7] <--> generator.gate;
}
