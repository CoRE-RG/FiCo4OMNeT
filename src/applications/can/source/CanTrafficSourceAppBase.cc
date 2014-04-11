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
    bitStuffingMethod = getParentModule()->par("bitStuffingMethod");
    bitStuffingPercentage = getParentModule()->par("bitStuffingPercentage");
    initialDataFrameCreation();
    initialRemoteFrameCreation();
}

void CanTrafficSourceAppBase::handleMessage(cMessage *msg) {
    CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
    dataFrameTransmission(df);
}

void CanTrafficSourceAppBase::initialRemoteFrameCreation() {

    if (getParentModule()->par("idRemoteFrames").stdstringValue() != "0") {
        cStringTokenizer remoteFrameIDsTokenizer(
                getParentModule()->par("idRemoteFrames"), ",");
        vector<int> remoteFrameIDs = remoteFrameIDsTokenizer.asIntVector();
        cStringTokenizer remoteFramesPeriodicityTokenizer(
                getParentModule()->par("periodicityRemoteFrames"), ",");

        cStringTokenizer dataLengthRemoteFramesTokenizer(
                getParentModule()->par("dataLengthRemoteFrames"), ",");

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
            if (can_msg->getPeriod() == 0) {
                EV<<"Remote frame with ID "<< can_msg->getCanID() << " has no period. Hence it will be ignored.\n";
            } else {
                scheduleAt(simTime() + (can_msg->getPeriod() / 1000.), can_msg);
            }

        }
    }
}

void CanTrafficSourceAppBase::initialDataFrameCreation() {
    if (getParentModule()->par("idDataFrames").stdstringValue() != "0") {
        cStringTokenizer dataFrameIDsTokenizer(
                getParentModule()->par("idDataFrames"), ",");
        vector<int> dataFrameIDs = dataFrameIDsTokenizer.asIntVector();

        cStringTokenizer dataFramesPeriodicityTokenizer(
                getParentModule()->par("periodicityDataFrames"), ",");

        cStringTokenizer dataLengthDataFramesTokenizer(
                getParentModule()->par("dataLengthDataFrames"), ",");

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
            if (can_msg->getPeriod() != 0) {
                scheduleAt(simTime() + (can_msg->getPeriod() / 1000.), can_msg);
            }
        }
    }
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
    return (arbFieldLength + DATAFRAMECONTROLBITS + (dataLength * 8) + calculateStuffingBits(dataLength, arbFieldLength));
}

int CanTrafficSourceAppBase::calculateStuffingBits(int dataLength, int arbFieldLength){
    switch(bitStuffingMethod){
        //no bitstuffing:
        case 0: return 0;
        //worst case:
        case 1: return ((CONTROLBITSFORBITSTUFFING + arbFieldLength + (dataLength * 8) - 1) / 4);  //Der hintere Teil steht fuer die Stopfbits
        break;
        //percentage:
        case 2: return (((CONTROLBITSFORBITSTUFFING + arbFieldLength + (dataLength * 8) - 1) / 4) * ((double)bitStuffingPercentage/100));
        break;
        //original:
        case 3: return 0; //TODO implement original bit stuffing
        break;
        default: return 0;
        break;
    }
}

void CanTrafficSourceAppBase::dataFrameTransmission(CanDataFrame *df) {
    CanDataFrame *outgoingFrame;
    if (df->isSelfMessage()) {
        outgoingFrame = df->dup();
        scheduleAt(simTime() + (df->getPeriod() / 1000.), df);
    } else if (df->arrivedOn("remoteIn")) {
        for (std::vector<CanDataFrame*>::iterator it = outgoingDataFrames.begin();
                it != outgoingDataFrames.end(); ++it) {
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
