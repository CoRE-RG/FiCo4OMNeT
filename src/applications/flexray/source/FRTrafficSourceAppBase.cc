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

#include "FRTrafficSourceAppBase.h"

void FRTrafficSourceAppBase::initialize() {
    initialDataFrameCreation();
}

void FRTrafficSourceAppBase::handleMessage(cMessage *msg) {
    FRFrame *frMsg = check_and_cast<FRFrame *>(msg);
    dataFrameTransmission(frMsg);
}

void FRTrafficSourceAppBase::initialDataFrameCreation() {
    if (getParentModule()->par("idDataFrames").stdstringValue() != "0") {
        cStringTokenizer dataFrameIDsTokenizer(
                getParentModule()->par("idDataFrames"), ",");
        vector<int> dataFrameIDs = dataFrameIDsTokenizer.asIntVector();

        cStringTokenizer dataFramesPeriodicityTokenizer(
                getParentModule()->par("periodicityDataFrames"), ",");

        cStringTokenizer dataLengthDataFramesTokenizer(
                getParentModule()->par("dataLengthDataFrames"), ",");

        for (unsigned int i = 0; i < dataFrameIDs.size(); i++) {
//            FRFrame *can_msg = new FRFrame("message");
//            can_msg->setNode(getParentModule()->par("node"));
//            can_msg->setCanID(checkAndReturnID(dataFrameIDs.at(i)));
//            can_msg->setLength(
//                    calculateLength(
//                            atoi(dataLengthDataFramesTokenizer.nextToken())));
//            can_msg->setPeriod(
//                    atoi(dataFramesPeriodicityTokenizer.nextToken()));
//            outgoingDataFrames.push_back(can_msg);
//            if (can_msg->getPeriod() != 0) {
//                scheduleAt(simTime() + (can_msg->getPeriod() / 1000.), can_msg);
//            }
        }
    }
}

int FRTrafficSourceAppBase::calculateLength(int dataLength) {
//    int arbFieldLength = 0;
//    if (canVersion.compare("2.0B") == 0) {
//        arbFieldLength += ARBITRATIONFIELD29BIT;
//    }
//    return (arbFieldLength + DATAFRAMECONTROLBITS + (dataLength * 8) + calculateStuffingBits(dataLength, arbFieldLength));
    return 0;
}

void FRTrafficSourceAppBase::dataFrameTransmission(FRFrame *df) {
    FRFrame *outgoingFrame;
    if (df->isSelfMessage()) {
        outgoingFrame = df->dup();
//        scheduleAt(simTime() + (df->getPeriod() / 1000.), df);
    } else if (df->arrivedOn("remoteIn")) {
        for (std::vector<FRFrame*>::iterator it = outgoingDataFrames.begin();
                it != outgoingDataFrames.end(); ++it) {
            FRFrame *tmp = *it;
            if (tmp->getId() == df->getId()) {
                outgoingFrame = tmp->dup();
                break;
            }
        }
        delete df;
    }
//    outgoingFrame->setStartTime(simTime());
    send(outgoingFrame, "out");
}
