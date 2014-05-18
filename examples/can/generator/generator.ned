//Copyright (c) 2014, CoRE Research Group, Hamburg University of Applied Sciences
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification,
//are permitted provided that the following conditions are met:
//
//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
//3. Neither the name of the copyright holder nor the names of its contributors
//   may be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
//ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

package fico4omnet.examples.can.generator;

import fico4omnet.bus.can.CanBus;
import fico4omnet.nodes.can.CanNode;

// Generic network configuration for CAN-Networks
// 
// Can be used for various number of CAN-Nodes, but the design of the network can't be changed. Used for all examples.
network generator
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
