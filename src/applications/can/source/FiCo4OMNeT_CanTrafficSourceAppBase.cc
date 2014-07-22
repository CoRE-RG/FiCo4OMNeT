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

#include "FiCo4OMNeT_CanTrafficSourceAppBase.h"

namespace FiCo4OMNeT {

Define_Module(CanTrafficSourceAppBase)
;

void CanTrafficSourceAppBase::initialize() {
    canVersion =
            getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->par(
                    "version").stdstringValue();
    bitStuffingPercentage =
            getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->par(
                    "bitStuffingPercentage");
    checkParameterValues();

    initialDataFrameCreation();
    initialRemoteFrameCreation();
}

void CanTrafficSourceAppBase::checkParameterValues() {
    if (bitStuffingPercentage < 0 || bitStuffingPercentage > 1) {
        throw cRuntimeError(
                "The value for the parameter \"bitStuffingPercentage\" is not permitted. Permitted values are from 0 to 1.");
    }
    if (canVersion.compare("2.0A") != 0 && canVersion.compare("2.0B") != 0) {
        throw cRuntimeError(
                "The value for the parameter \"canVersion\" is not permitted. Permitted values are \"2.0B\" and \"2.0A\".");
    }
}

void CanTrafficSourceAppBase::handleMessage(cMessage *msg) {
    CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
    dataFrameTransmission(df);
}

void CanTrafficSourceAppBase::initialRemoteFrameCreation() {

    if (par("idRemoteFrames").stdstringValue() != "0") {
        cStringTokenizer remoteFrameIDsTokenizer(par("idRemoteFrames"), ",");
        std::vector<int> remoteFrameIDs = remoteFrameIDsTokenizer.asIntVector();
        cStringTokenizer remoteFramesPeriodicityTokenizer(
                par("periodicityRemoteFrames"), ",");

        cStringTokenizer dataLengthRemoteFramesTokenizer(
                par("dataLengthRemoteFrames"), ",");

        cStringTokenizer initialRemoteFrameOffsetTokenizer(
                par("initialRemoteFrameOffset"), ",");

        for (unsigned int i = 0; i < remoteFrameIDs.size(); i++) {
            if (!dataLengthRemoteFramesTokenizer.hasMoreTokens()) {
                throw cRuntimeError(
                        "No more values for the remote frame data length for the next remote frame ID. Configuration in the ini file may be incorrect.");
            }
            if (!remoteFramesPeriodicityTokenizer.hasMoreTokens()) {
                throw cRuntimeError(
                        "No more values for the remote frame period for the next remote frame ID. Configuration in the ini file may be incorrect.");
            }
            if (!remoteFramesPeriodicityTokenizer.hasMoreTokens()) {
                throw cRuntimeError(
                        "No more values for the remote frame offset for the next remote frame ID. Configuration in the ini file may be incorrect.");
            }
            CanDataFrame *can_msg = new CanDataFrame("remoteFrame");
            can_msg->setCanID(checkAndReturnID(remoteFrameIDs.at(i)));
            can_msg->setLength(
                    calculateLength(
                            atoi(dataLengthRemoteFramesTokenizer.nextToken())));
            can_msg->setRtr(true);
            can_msg->setPeriod(
                    atoi(remoteFramesPeriodicityTokenizer.nextToken()));
            registerRemoteFrameAtPort(can_msg->getCanID());
            if (can_msg->getPeriod() == 0) {
                EV<< "Remote frame with ID " << can_msg->getCanID()
                << " has no period. Hence it will be ignored.\n";
                if (initialRemoteFrameOffsetTokenizer.hasMoreTokens()) {
                    initialRemoteFrameOffsetTokenizer.nextToken();
                }
            } else {
                double offset;
                initialRemoteFrameOffsetTokenizer.hasMoreTokens() ?
                offset = atof(
                        initialRemoteFrameOffsetTokenizer.nextToken()) :
                offset = 0;
                scheduleAt(
                        simTime() + SimTime(offset)
                        + SimTime(
                                par("periodInaccurracy").doubleValue()),
                        can_msg);
            }

        }
        if (dataLengthRemoteFramesTokenizer.hasMoreTokens()) {
            EV<< "There are more values defined for the data length. Please check your configuration files.";
        }
        if (remoteFramesPeriodicityTokenizer.hasMoreTokens()) {
            EV<< "There are more values defined for the period. Please check your configuration files.";
        }
        if (initialRemoteFrameOffsetTokenizer.hasMoreTokens()) {
            EV<< "There are more values defined for the remote frame offset. Please check your configuration files.";
        }
    }
}

void CanTrafficSourceAppBase::registerRemoteFrameAtPort(int canID) {
    CanPortInput* port = (CanPortInput*) getParentModule()->getSubmodule(
            "canNodePort")->getSubmodule("canPortInput");
    port->registerOutgoingRemoteFrame(canID);
}

void CanTrafficSourceAppBase::initialDataFrameCreation() {
    if (par("idDataFrames").stdstringValue() != "0") {
        cStringTokenizer dataFrameIDsTokenizer(par("idDataFrames"), ",");
        std::vector<int> dataFrameIDs = dataFrameIDsTokenizer.asIntVector();

        cStringTokenizer dataFramesPeriodicityTokenizer(
                par("periodicityDataFrames"), ",");

        cStringTokenizer dataLengthDataFramesTokenizer(
                par("dataLengthDataFrames"), ",");

        cStringTokenizer initialDataFrameOffsetTokenizer(
                par("initialDataFrameOffset"), ",");

        for (unsigned int i = 0; i < dataFrameIDs.size(); i++) {

            if (!dataLengthDataFramesTokenizer.hasMoreTokens()) {
                throw cRuntimeError(
                        "No more values for the data frame data length for the next data frame ID. Configuration in the ini file may be incorrect.");
            }
            if (!dataFramesPeriodicityTokenizer.hasMoreTokens()) {
                throw cRuntimeError(
                        "No more values for the data frame period for the next data frame ID. Configuration in the ini file may be incorrect.");
            }
            if (!initialDataFrameOffsetTokenizer.hasMoreTokens()) {
                throw cRuntimeError(
                        "No more values for the data frame offset for the next data frame ID. Configuration in the ini file may be incorrect.");
            }
            CanDataFrame *can_msg = new CanDataFrame("message");
            can_msg->setCanID(checkAndReturnID(dataFrameIDs.at(i)));
            can_msg->setLength(
                    calculateLength(
                            atoi(dataLengthDataFramesTokenizer.nextToken())));
            can_msg->setPeriod(
                    atoi(dataFramesPeriodicityTokenizer.nextToken()));
            outgoingDataFrames.push_back(can_msg);
            registerDataFrameAtPort(can_msg->getCanID());
            if (can_msg->getPeriod() != 0) {
                double offset;
                initialDataFrameOffsetTokenizer.hasMoreTokens() ?
                        offset = atof(
                                initialDataFrameOffsetTokenizer.nextToken()) :
                        offset = 0;
                scheduleAt(
                        simTime() + SimTime(offset)
                                + SimTime(
                                        par("periodInaccurracy").doubleValue()),
                        can_msg);
            } else {
                if (initialDataFrameOffsetTokenizer.hasMoreTokens()) {
                    initialDataFrameOffsetTokenizer.nextToken();
                }
            }
        }
        if (dataLengthDataFramesTokenizer.hasMoreTokens()) {
            EV<< "There are more values defined for the data frame data length. Please check your configuration files.";
        }
        if (dataFramesPeriodicityTokenizer.hasMoreTokens()) {
            EV<< "There are more values defined for the data frame period. Please check your configuration files.";
        }
        if (initialDataFrameOffsetTokenizer.hasMoreTokens()) {
            EV<< "There are more values defined for the data frame offset. Please check your configuration files.";
        }

    }
}

void CanTrafficSourceAppBase::registerDataFrameAtPort(int canID) {
    CanPortInput* port = (CanPortInput*) getParentModule()->getSubmodule(
            "canNodePort")->getSubmodule("canPortInput");
    port->registerOutgoingDataFrame(canID, this->gate("remoteIn"));
}

int CanTrafficSourceAppBase::checkAndReturnID(int id) {
    if (canVersion.compare("2.0A") == 0) {
        if (id < 0 || id > VERSIONAMAX) {
            EV<< "ID " << id << " not valid." << endl;
            endSimulation();
        }
    } else {
        if (id < 0 || id > VERSIONBMAX) {
            EV << "ID " << id << " not valid." << endl;
            endSimulation();
        }
    }
    return id;
}

int CanTrafficSourceAppBase::calculateLength(int dataLength) {
    int arbFieldLength = 0;
    if (canVersion.compare("2.0B") == 0) {
        arbFieldLength += ARBITRATIONFIELD29BIT;
    }
    return (arbFieldLength + DATAFRAMECONTROLBITS + (dataLength * 8)
            + calculateStuffingBits(dataLength, arbFieldLength));
}

int CanTrafficSourceAppBase::calculateStuffingBits(int dataLength,
        int arbFieldLength) {
    return (((CONTROLBITSFORBITSTUFFING + arbFieldLength + (dataLength * 8) - 1)
            / 4) * bitStuffingPercentage);
}

void CanTrafficSourceAppBase::dataFrameTransmission(CanDataFrame *df) {
    CanDataFrame *outgoingFrame;
    if (df->isSelfMessage()) {
        outgoingFrame = df->dup();
        scheduleAt(
                simTime() + (df->getPeriod() / 1000.)
                        + SimTime(par("periodInaccurracy").doubleValue()), df);
    } else if (df->arrivedOn("remoteIn")) {
        for (std::vector<CanDataFrame*>::iterator it =
                outgoingDataFrames.begin(); it != outgoingDataFrames.end();
                ++it) {
            CanDataFrame *tmp = *it;
            if (tmp->getCanID() == df->getCanID()) {
                outgoingFrame = tmp->dup();
                break;
            }
        }
        delete df;
    }
    outgoingFrame->setStartTime(simTime());
    send(outgoingFrame, "out");
}

}
