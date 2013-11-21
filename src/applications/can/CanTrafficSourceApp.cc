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

#include "CanTrafficSourceApp.h"

CanTrafficSourceApp::CanTrafficSourceApp() {
    // TODO Auto-generated constructor stub

}

CanTrafficSourceApp::~CanTrafficSourceApp() {
    // TODO Auto-generated destructor stub
}

void CanTrafficSourceApp::initialize() {
//    initialRemoteFrameCreation();//TODO wieder reinmachen
    initialDataFrameCreation();
    canVersion =getParentModule()->par("version").str();
//    canVersion = "2.0A";
}

void CanTrafficSourceApp::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
        CanDataFrame *outgoingFrame = df->dup();
        outgoingFrame->setStartTime(simTime());
        send(outgoingFrame, "out");
        scheduleAt(simTime() + (df->getPeriod() / 1000.), df);
    }
}

void CanTrafficSourceApp::initialRemoteFrameCreation() {
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
        can_msg->setCanID(checkAndReturnID(remoteFrameIDs.at(i+1)));
        can_msg->setLength(calculateLength(atoi(dataLengthRemoteFramesTokenizer.nextToken())));
        can_msg->setRtr(true);
        can_msg->setPeriod(atoi(remoteFramesPeriodicityTokenizer.nextToken()));
        outgoingRemoteFrames.push_back(can_msg); // TODO brauch ich das?
        scheduleAt(simTime() + (can_msg->getPeriod() / 1000.), can_msg);
    }
}

void CanTrafficSourceApp::initialDataFrameCreation() {
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
        can_msg->setLength(calculateLength(atoi(dataLengthDataFramesTokenizer.nextToken())));
        can_msg->setPeriod(atoi(dataFramesPeriodicityTokenizer.nextToken()));
        outgoingDataFrames.push_back(can_msg); // TODO brauch ich das?
        scheduleAt(simTime() + (can_msg->getPeriod() / 1000.), can_msg);
    }
}

int CanTrafficSourceApp::checkAndReturnID(int id) {
    if (canVersion.compare("2.0A")) {           //2.0A
        if (id < 0 || id > VERSIONAMAX) {
            EV<< "ID " << id << " not valid." << endl;
            endSimulation();
        }
    } else {                                    //2.0B
        if(id < 0 || id > VERSIONBMAX) {
            EV << "ID " << id << " not valid." << endl;
            endSimulation();
        }
    }
    return id;
}

int CanTrafficSourceApp::calculateLength(int dataLength){
    int frameLength = 0;
    if (canVersion.compare("2.0B") == 0) {
        frameLength += ARBITRATIONFIELD29BIT;
    }
    return frameLength + DATAFRAMEOVERHEAD + (dataLength<<3); //TODO + StuffingBits
}
