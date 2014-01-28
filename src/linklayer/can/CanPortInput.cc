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

#include "CanPortInput.h"

void CanPortInput::initialize() {

    cStringTokenizer idIncomingFramesTokenizer(
            getParentModule()->getParentModule()->par("idIncomingFrames"), ",");
    incomingDataFrameIDs = idIncomingFramesTokenizer.asIntVector();
    cStringTokenizer idIncomingRemoteFramesTokenizer(
            getParentModule()->getParentModule()->par("idDataFrames"), ",");
    outgoingDataFrameIDs = idIncomingRemoteFramesTokenizer.asIntVector();
    cStringTokenizer idOutgoingRemoteFramesTokenizer(
            getParentModule()->getParentModule()->par("idRemoteFrames"), ",");
    outgoingRemoteFrameIDs = idIncomingRemoteFramesTokenizer.asIntVector();

    bandwidth = getParentModule()->getParentModule()->par("bandwidth");
    errors = getParentModule()->getParentModule()->par("errors");
    errorperc = getParentModule()->getParentModule()->par("errorperc");

    scheduledDataFrame = new CanDataFrame();
    scheduledErrorFrame = new ErrorFrame();

    //TODO stats
//    messagestats.push_back(new Stats(id)); //Statistiken f�r diese Nachricht erstellen
}

void CanPortInput::handleMessage(cMessage *msg) {
    take(msg);
    std::string msgClass = msg->getClassName();
    if (msg->isSelfMessage()) {
        if (msgClass.compare("ErrorFrame") == 0) {
            ErrorFrame *ef = check_and_cast<ErrorFrame *>(msg);
            forwardOwnErrorFrame(ef);
        } else {
            CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
            forwardDataFrame(df);
        }
    } else if (msgClass.compare("CanDataFrame") == 0) {
        CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
        int rcverr = intuniform(0, 99);
        if (errors && (rcverr < errorperc)) {
            handleError(df);
        } else {
            receiveMessage(df);
        }
    } else if (msgClass.compare("ErrorFrame") == 0) {
        ErrorFrame *ef = check_and_cast<ErrorFrame *>(msg);
        handleExternErrorFrame(ef);
    }
    delete msg;
}

void CanPortInput::receiveMessage(CanDataFrame *df) {
    int frameLength = df->getLength();
    if (checkExistence(df)) {
        scheduledDataFrame = df->dup();
        scheduleAt((simTime() + calculateScheduleTiming(frameLength)),
                scheduledDataFrame);
    }
}

void CanPortInput::handleError(CanDataFrame *df) {
    ErrorFrame *errorMsg = new ErrorFrame("senderror");
    int pos = intuniform(0, df->getLength() - 12); //Position zwischen 0 - L�nge des Frames (abz�glich ((EOF und ACK-Delimiter)+1))
    errorMsg->setKind(intuniform(0, 1)); //0: Bit-Error, 1: Form-Error
    errorMsg->setCanID(df->getCanID());
    if (pos > 0)
        pos--;  //wegen der verschobenen Sendezeiten
    errorMsg->setPos(pos);
    scheduledErrorFrame = errorMsg;
    scheduleAt((simTime() + calculateScheduleTiming(pos)), scheduledErrorFrame);
//    scheduleAt((simTime() + calculateScheduleTiming(pos)), errorMsg->dup());
}

bool CanPortInput::checkExistence(CanDataFrame *df) {
    if (df->getRtr()) {
        return checkOutgoingDataFrames(df->getCanID());
    } else {
        return checkIncomingDataFrames(df->getCanID());
    }
}

bool CanPortInput::checkOutgoingDataFrames(int id) {
    for (std::vector<int>::iterator it = outgoingDataFrameIDs.begin();
            it != outgoingDataFrameIDs.end(); ++it) {
        if (*it == id) {
            return true;
        }
    }
    return false;
}

bool CanPortInput::checkOutgoingRemoteFrames(int id) {
    for (std::vector<int>::iterator it = outgoingRemoteFrameIDs.begin();
            it != outgoingRemoteFrameIDs.end(); ++it) {
        if (*it == id) {
            return true;
        }
    }
    return false;
}

bool CanPortInput::checkIncomingDataFrames(int id) {
    for (std::vector<int>::iterator it = incomingDataFrameIDs.begin();
            it != incomingDataFrameIDs.end(); ++it) {
        if (*it == id) {
            return true;
        }
    }
    return false;
}

double CanPortInput::calculateScheduleTiming(int length) {
    return ((double) length) / bandwidth;
}

void CanPortInput::forwardDataFrame(CanDataFrame *df) {
    if (df->getRtr()) {
        for (std::vector<int>::iterator it = outgoingDataFrameIDs.begin();
                it != outgoingDataFrameIDs.end(); ++it) {
            if (*it == df->getCanID()) {
                cModule* sourceApp =
                        getParentModule()->getParentModule()->getSubmodule(
                                "sourceApp");
                sendDirect(df->dup(), sourceApp, "remoteIn");
                break;
            }
        }
    } else {
        for (std::vector<int>::iterator it = incomingDataFrameIDs.begin();
                it != incomingDataFrameIDs.end(); ++it) {
            if (*it == df->getCanID()) {
                send(df->dup(), "out");
                break;
            }
        }
    }
}

void CanPortInput::forwardOwnErrorFrame(ErrorFrame *ef) {
    cModule* portOutput = getParentModule()->getSubmodule("canPortOutput");
    sendDirect(ef, portOutput, "directIn");
}

void CanPortInput::handleExternErrorFrame(ErrorFrame *ef) {
    if (checkOutgoingDataFrames(ef->getCanID()) || checkOutgoingRemoteFrames(ef->getCanID())) {
        CanPortOutput* portOutput = (CanPortOutput*)getParentModule()->getSubmodule("canPortOutput");
        portOutput->handleReceivedErrorFrame();
        // dieser knoten ist sender; ef an output; da evtl. geschedulte ef löschen & neue Arbitrierung
    }
    cancelEvent(scheduledDataFrame);
    cancelEvent(scheduledErrorFrame);
}
