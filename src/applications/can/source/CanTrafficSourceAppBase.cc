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

#include "CanTrafficSourceAppBase.h"

void CanTrafficSourceAppBase::initialize() {
    canVersion = getParentModule()->par("version").stdstringValue();
    bitStuffingPercentage = getParentModule()->par("bitStuffingPercentage");
    checkParameterValues();

    initialDataFrameCreation();
    initialRemoteFrameCreation();
}

void CanTrafficSourceAppBase::checkParameterValues() {
    if (bitStuffingPercentage < 0 || bitStuffingPercentage > 1) {
        EV<< "The value for the parameter \"bitStuffingPercentage\" is not permitted. Permitted values are from 0 to 1.";
        endSimulation();
    }
    if (canVersion.compare("2.0A") != 0 && canVersion.compare("2.0B") != 0) {
        EV << "The value for the parameter \"canVersion\" is not permitted. Permitted values are \"2.0B\" and \"2.0A\".";
        endSimulation();
    }
}

void CanTrafficSourceAppBase::handleMessage(cMessage *msg) {
    CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
    dataFrameTransmission(df);
//    if (CanDataFrame *df = dynamic_cast<CanDataFrame *>(msg)) {
//        dataFrameTransmission(df);
//    } else {
//        registerDataFrameAtPort(1);
//    }
}

void CanTrafficSourceAppBase::initialRemoteFrameCreation() {

    if (par("idRemoteFrames").stdstringValue() != "0") {
        cStringTokenizer remoteFrameIDsTokenizer(par("idRemoteFrames"), ",");
        vector<int> remoteFrameIDs = remoteFrameIDsTokenizer.asIntVector();
        cStringTokenizer remoteFramesPeriodicityTokenizer(
                par("periodicityRemoteFrames"), ",");

        cStringTokenizer dataLengthRemoteFramesTokenizer(
                par("dataLengthRemoteFrames"), ",");

        for (unsigned int i = 0; i < remoteFrameIDs.size(); i++) {
            CanDataFrame *can_msg = new CanDataFrame("remoteFrame");
            can_msg->setNode(getParentModule()->par("node"));
            can_msg->setCanID(checkAndReturnID(remoteFrameIDs.at(i)));
            can_msg->setLength(
                    calculateLength(
                            atoi(dataLengthRemoteFramesTokenizer.nextToken())));
            can_msg->setRtr(true);
            can_msg->setPeriod(
                    atoi(remoteFramesPeriodicityTokenizer.nextToken()));
            registerRemoteFrameAtPort(can_msg->getCanID());
            if (can_msg->getPeriod() == 0) {
                EV<<"Remote frame with ID "<< can_msg->getCanID() << " has no period. Hence it will be ignored.\n";
            } else {
                scheduleAt(simTime() + (can_msg->getPeriod() / 1000.), can_msg);
            }

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
        cStringTokenizer dataFrameIDsTokenizer(
                par("idDataFrames"), ",");
        vector<int> dataFrameIDs = dataFrameIDsTokenizer.asIntVector();

        cStringTokenizer dataFramesPeriodicityTokenizer(
                par("periodicityDataFrames"), ",");

        cStringTokenizer dataLengthDataFramesTokenizer(
                par("dataLengthDataFrames"), ",");

        for (unsigned int i = 0; i < dataFrameIDs.size(); i++) {
            CanDataFrame *can_msg = new CanDataFrame("message");
            can_msg->setNode(getParentModule()->par("node"));
            can_msg->setCanID(checkAndReturnID(dataFrameIDs.at(i)));
            can_msg->setLength(
                    calculateLength(
                            atoi(dataLengthDataFramesTokenizer.nextToken())));
            can_msg->setPeriod(
                    atoi(dataFramesPeriodicityTokenizer.nextToken()));
            outgoingDataFrames.push_back(can_msg);
            registerDataFrameAtPort(can_msg->getCanID());
            if (can_msg->getPeriod() != 0) {
                scheduleAt(simTime() + (can_msg->getPeriod() / 1000.), can_msg);
            }
        }
    }
}

void CanTrafficSourceAppBase::registerDataFrameAtPort(int canID) {
    CanPortInput* port = (CanPortInput*) getParentModule()->getSubmodule(
            "canNodePort")->getSubmodule("canPortInput");
    port->registerOutgoingDataFrame(canID,this->gate("remoteIn"));
}

int CanTrafficSourceAppBase::checkAndReturnID(int id) {
    if (canVersion.compare("2.0A") == 0) {
        if (id < 0 || id > VERSIONAMAX) {
            EV<< "ID " << id << " not valid." << endl;
            endSimulation();
        }
    } else {
        if(id < 0 || id > VERSIONBMAX) {
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
        scheduleAt(simTime() + (df->getPeriod() / 1000.), df);
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
