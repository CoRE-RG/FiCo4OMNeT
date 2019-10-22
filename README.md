# FiCo4OMNeT

FiCo4OMNeT is an open source simulation model for the event-based simulation of fieldbus technologies in the [OMNEST/OMNeT++](https://omnetpp.org/) simulation system. It enables the simulation of CAN and Flexray technologies.

<img src="/doc/images/fico4omnet.png" alt="FiCo4OMNeT Environment" width="35%">


## Features

### CAN
It is possible to create networks with traffic consisting of Data-, Remote- and Error-Frames. To get some variation in the transmit and receive times of cyclic messages it is possible to simulate a clock drift.

The arbitration ensures that the frame with the highest priority is transmitted as soon as several nodes try to transmit their messages.


### FlexRay
Each node can transmit messages in the assigned slots within the static and the dynamic segment. Based on the configuration the frame is transmitted on channel A, channel B or on both. Every member of the network has an own clock. Based on the configuration each clock has an inaccuracy. The implemented synchronisation compensates theses drifts.


## History
FiCo4OMNeT was first introduced as !FlexRay simulation model at the [6th International Workshop on OMNeT++ (2013)](http://www.omnet-workshop.org/2013/) along with the [6th International ICST Conference on Simulation Tools and Techniques (2013)](http://www.simutools.org/2013/). You can find the [publication](http://core.informatik.haw-hamburg.de/images/publications/papers/bsks-stafc-13a.pdf) and [slides](http://core.informatik.haw-hamburg.de/images/publications/papers/bsks-stafc-13b.pdf) here.

To merge the simulation models of CAN and !FlexRay significant changes were necessary. The result of this merging process is the FieldBus for OMNeT++ Simulation model (FiCo4OMNeT).


## References
If you would like to reference this model please use the following [publication](http://core.informatik.haw-hamburg.de/images/publications/papers/bsks-stafc-13a.pdf):

S. Buschmann, T. Steinbach, F. Korf, and T. C. Schmidt. Simulation based Timing Analysis of FlexRay Communication at System Level. In Proceedings of the 6th International ICST Conference on Simulation Tools and Techniques, pages 285-290, New York, Mar. 2013. ACM-DL.


## Quick Start
1. Download OMNeT++ 5.5.1
    * [https://omnetpp.org/download/](https://omnetpp.org/download/)
2. Install OMNeT++
    * [https://doc.omnetpp.org/omnetpp/InstallGuide.pdf](https://doc.omnetpp.org/omnetpp/InstallGuide.pdf)
3. Get INET framework 3.6.6 (optional)
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
* [CoRE simulation models overview](https://core-researchgroup.de/projects/simulation.html)
* [Abstract Network Description Language (ANDL) overview](https://core-researchgroup.de/projects/simulation/abstract-network-description-language.html)

### Installation
Please see [INSTALL](/INSTALL)

### Documentation
Please see [doc/](/doc)

## IMPORTANT
The FiCo4OMNeT model is under continuous development: new parts are added, bugs are corrected, and so on. We cannot assert that the implementation will work fully according to the specifications. YOU ARE RESPONSIBLE YOURSELF TO MAKE SURE THAT THE MODELS YOU USE IN YOUR SIMULATIONS WORK CORRECTLY, AND YOU'RE GETTING VALID RESULTS.
