//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//
// @titlepage
//
//<center><img src="../images/CoRE.png"/><img src="../images/HAW.gif"/></center>
//
// <center><h1>FiCo4OMNeT: Fieldbus Communication for OMNeT++</h1></center>
// <center>release [VERSION_STRING]</center>
//
// In this model the bus technologies Controller Area Network (CAN) and FlexRay are implemented in the OMNEST/OMNeT++ simulation system.
// It was created by the <a href="http://core.informatik.haw-hamburg.de/" target="_top">CoRE (Communication over Realtime Ethernet)</a> research group
// at the <a href="http://www.haw-hamburg.de" target="_top">HAW-Hamburg (Hamburg University of Applied Sciences) </a>.
//
//
// <h2>Requirements</h2>
// - <a href="http://omnetpp.org/" target="_top">OMNeT++</a> (minimum version 5.0)
//
//
// <h2>Related documentation</h2>
//   - <a href="../doxy/index.html" target="_top">C++ source documentation (Doxygen)</a>
//
// <h2>Background information</h2>
// Informations about the arbitration and the error functionality of a Can network.
//   - <a href="../neddoc/background.html" target="_top">Background</a>
//
// <h2>IMPORTANT</h2>
// The bus model is continuously being improved: new parts
// are added, bugs are corrected, and so on. We cannot assert that the implementation
// will work fully according to the specifications. YOU ARE RESPONSIBLE YOURSELF
// TO MAKE SURE THAT THE MODELS YOU USE IN YOUR SIMULATIONS WORK CORRECTLY, AND YOU'RE
// GETTING VALID RESULTS.
//
// <h2>Contributions</h2>
// Contributions are highly welcome. Please contact us if you would like to participate!
//
// <h3>Contributors</h3>
// - Till Steinbach (HAW-Hamburg)
// - Stefan Buschmann (HAW-Hamburg)
//
//
// <h2>Contact</h2>
// CoRE Research Group
// <a href="mailto:core@informatik.haw-hamburg.de">core@informatik.haw-hamburg.de</a>
//
//
// @page background.html, Background
// <center><h1>FiCo4OMNeT: Background</h1></center>
//
//This page is about some functionalities of the Can framework.
//
// <h2>Arbitration</h2>
//The arbitration in this model is implemented in a simplified version compared to the
//specification. The frames are transmitted in the correct order but it does not take place
//on the physical layer. Instead the CanBusLogic manages the arbitration.
//
//As soon as a node wants to send a message, it registers the CanID
//at the CanBusLogic. The BusLogic grants the node with the highest priority the
//permission to send. Once the transmission is completed the BusLogic module
//grants the permission to the next node with the highest registered CanID. If
//there are no further messages scheduled, the bus waits until the next
//registration.
//
// <h2>Error Frames</h2>
//The error functionality can be activated by configuring a value greater than 0 for the 
//errorperc parameter in the CanNode.
//There are two kinds of errors: Sending-Errors and Receive-Errors.
//
//The parameter errorperc, which can be defined for each node in the ini file,
//represents the percentage chance that an error will occur. This error will occur
//at a random time within the transmission. At the given time, the error frame is
//sent to the bus and subsequently forwarded to all nodes.
//
//If a node receives a data or remote frame the errorperc parameter is used to
//generate a receive error. The error transmission works equally to the send
//error.
//
//As soon as a node receives an error frame the reception of the current data or
//remote frame is aborted. So the faulty frame is not processed by the node. When
//the transmission of the error frame is completed a new sending permission is
//granted to the node with the highest priority.
//

package fico4omnet;
