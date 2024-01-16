# FiCo4OMNeT

FiCo4OMNeT is an open source simulation model for the event-based simulation of fieldbus technologies in the [OMNEST/OMNeT++](https://omnetpp.org/) simulation system. It enables the simulation of CAN and Flexray technologies.

<img src="/doc/images/fico4omnet.png" alt="FiCo4OMNeT Environment" width="35%">


## History
FiCo4OMNeT was first introduced as FlexRay simulation model at the 6th International Workshop on OMNeT++ (2013) along with the 6th International ICST Conference on Simulation Tools and Techniques (2013). You can find the [publication](https://core.inet.haw-hamburg.de/assets/bibliography/eigene/bsks-stafc-13.pdf) and [slides](https://core.inet.haw-hamburg.de/assets/bibliography/eigene/bsks-stafc-13_Slides.pdf) here.

To merge the simulation models of CAN and FlexRay significant changes were necessary. The result of this merging process is the FieldBus for OMNeT++ Simulation model (FiCo4OMNeT).


## Features

### CAN
It is possible to create networks with traffic consisting of Data-, Remote- and Error-Frames. To get some variation in the transmit and receive times of cyclic messages it is possible to simulate a clock drift.

The arbitration ensures that the frame with the highest priority is transmitted as soon as several nodes try to transmit their messages.


### FlexRay
Each node can transmit messages in the assigned slots within the static and the dynamic segment. Based on the configuration the frame is transmitted on channel A, channel B or on both. Every member of the network has an own clock. Based on the configuration each clock has an inaccuracy. The implemented synchronisation compensates theses drifts.


## Reference
If you would like to reference this project please use the following [publication](https://link.springer.com/chapter/10.1007/978-3-030-12842-5_10):

Philipp Meyer, Franz Korf, Till Steinbach, Thomas C. Schmidt, Simulation of Mixed Critical In-vehicular Networks, In: Recent Advances in Network Simulation, (Antonio Virdis, Michael Kirsche Ed.), ser. EAI/Springer Innovations in Communication and Computing, May 2019. [https://doi.org/10.1007/978-3-030-12842-5_10](https://doi.org/10.1007/978-3-030-12842-5_10) 

* [arXiv preprint](https://arxiv.org/abs/1808.03081)


## Quick Start
1. Download OMNeT++ 6.0.2
    * [https://omnetpp.org/download/](https://omnetpp.org/download/)
2. Install OMNeT++
    * [https://doc.omnetpp.org/omnetpp/InstallGuide.pdf](https://doc.omnetpp.org/omnetpp/InstallGuide.pdf)
3. Get INET framework 3.8.3 (optional)
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
<a><img src="https://jenkins.core-rg.de/buildStatus/icon?job=FiCo4OMNeT/FiCo4OMNeT"></a>
* Tests:
<a><img src="https://jenkins.core-rg.de/buildStatus/icon?job=FiCo4OMNeT/FiCo4OMNeT_tests"></a>


<table>
  <tr>
    <th></th>
    <th>Ubuntu 22.04</th>
    <th>Windows 11</th>
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
* [CoRE simulation models overview](https://core-researchgroup.de/projects/simulation.html)
* [Abstract Network Description Language (ANDL) overview](https://core-researchgroup.de/projects/simulation/abstract-network-description-language.html)

### Installation
Please see [INSTALL](/INSTALL)

### Documentation
Please see [doc/](/doc)

## IMPORTANT
The FiCo4OMNeT model is under continuous development: new parts are added, bugs are corrected, and so on. We cannot assert that the implementation will work fully according to the specifications. YOU ARE RESPONSIBLE YOURSELF TO MAKE SURE THAT THE MODELS YOU USE IN YOUR SIMULATIONS WORK CORRECTLY, AND YOU'RE GETTING VALID RESULTS.
