# FiCo4OMNeT

FiCo4OMNeT is an open source simulation model for the event-based simulation of fieldbus technologies in the [OMNEST/OMNeT++](https://omnetpp.org/) simulation system. It enables the simulation of CAN and Flexray technologies.

<img src="/doc/images/fico4omnet.png" alt="FiCo4OMNeT Environment" width="35%">


## Quick Start
1. Download OMNeT++ 5.1.1 or 5.4.1
    * [https://omnetpp.org/download/old](https://omnetpp.org/download/old)
2. Install OMNeT++
    * [https://doc.omnetpp.org/omnetpp/InstallGuide.pdf](https://doc.omnetpp.org/omnetpp/InstallGuide.pdf)
3. Get INET framework 3.6.5 (optional)
    * [https://inet.omnetpp.org/Download.html](https://inet.omnetpp.org/Download.html)
4. Install CoRE plugins (optional)
    * OMNEST/OMNeT++ -> Help -> Install New Software...
    * URL `http://sim.core-rg.de/updates/`
    * Check [Abstract Network Description Language] | [CoRE Simulation Model Installer] | [Gantt Chart Timing Analyzer]
5. Get CoRE framework (GitHub or CoRE Simulation Model Installer)
    * GitHub: Clone framework and import it in OMNEST/OMNeT++
    * CoRE Simulation Model Installer: OMNEST/OMNeT++ -> Help -> Install CoRE Simulation Models...
6. Working with the framework
    * See the documentation in [doc/](/doc)
    * Start the examples in the framework
    

## Continuous Integration

The build state of the master branch is monitored:
* Building:
<p><img src="https://jenkins.core-rg.de/buildStatus/icon?job=FiCo4OMNeT/FiCo4OMNeT"></p>
* Tests:
<p><img src="https://jenkins.core-rg.de/buildStatus/icon?job=FiCo4OMNeT/FiCo4OMNeT_tests"></p>


<table>
  <tr>
    <th></th>
    <th>Ubuntu 18.04</th>
    <th>Windows 10</th>
  </tr>
  <tr>
    <td>Building</td>
    <td><img src="https://jenkins.core-rg.de/buildStatus/icon?job=FiCo4OMNeT/FiCo4OMNeT/Nodes=Ubuntu_18.04"></td>
    <td><img src="https://jenkins.core-rg.de/buildStatus/icon?job=FiCo4OMNeT/FiCo4OMNeT/Nodes=Windows_10"></td>
  </tr>
  <tr>
    <td>Tests</td>
    <td><img src="https://jenkins.core-rg.de/buildStatus/icon?job=FiCo4OMNeT/FiCo4OMNeT_tests/Nodes=Ubuntu_18.04"></td>
    <td><img src="https://jenkins.core-rg.de/buildStatus/icon?job=FiCo4OMNeT/FiCo4OMNeT_tests/Nodes=Windows_10"></td>
  </tr>
</table>


## Further Information

# Installation
Please see [INSTALL](/INSTALL)

# Documentation
Please see [doc/](/doc)

## IMPORTANT
The FiCo4OMNeT model is under continuous development: new parts are added, bugs are corrected, and so on. We cannot assert that the implementation will work fully according to the specifications. YOU ARE RESPONSIBLE YOURSELF TO MAKE SURE THAT THE MODELS YOU USE IN YOUR SIMULATIONS WORK CORRECTLY, AND YOU'RE GETTING VALID RESULTS.
