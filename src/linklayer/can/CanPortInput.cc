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

    bandwidth = getParentModule()->getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->par("bandwidth");
    errorsActivated = getParentModule()->getParentModule()->par("errorsActivated");
    errorperc = getParentModule()->getParentModule()->par("errorperc");

    scheduledDataFrame = NULL;
    scheduledErrorFrame = NULL;

    rcvdDFSignal = registerSignal("receivedCompleteDF");
    rcvdRFSignal = registerSignal("receivedCompleteRF");

//    messagestats.push_back(new Stats(id)); //Statistiken fï¿½r diese Nachricht erstellen
}

void CanPortInput::finish() {

}

void CanPortInput::handleMessage(cMessage *msg) {
    std::string msgClass = msg->getClassName();
    if (msg->isSelfMessage()) {
        if (msgClass.compare("ErrorFrame") == 0) {
            ErrorFrame *ef = dynamic_cast<ErrorFrame *>(msg);
            forwardOwnErrorFrame(ef);
            if (scheduledErrorFrame != NULL) {
                cancelEvent(scheduledErrorFrame);
            }
            delete (scheduledErrorFrame);
            scheduledErrorFrame = NULL;
        } else if (msgClass.compare("CanDataFrame") == 0) {
            CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
            forwardDataFrame(df);
            if (scheduledDataFrame != NULL) {
                cancelEvent(scheduledDataFrame);
            }
            delete (scheduledDataFrame);
            scheduledDataFrame = NULL;
        }
    } else if (msgClass.compare("CanDataFrame") == 0) {
        CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
        if (checkExistence(df)) {
            int rcverr = intuniform(0, 99);
            if (errorsActivated && (rcverr < errorperc)) {
                generateReceiveError(df);
            } else {
                receiveMessage(df);
            }
        } else {
            if (scheduledDataFrame != NULL) {
                cancelEvent(scheduledDataFrame);
            }
            dropAndDelete(scheduledDataFrame);
            scheduledDataFrame = NULL;
        }
        delete msg;
    } else if (msgClass.compare("ErrorFrame") == 0) {
        ErrorFrame *ef = check_and_cast<ErrorFrame *>(msg);
        handleExternErrorFrame(ef);
        delete msg;
    }
}

void CanPortInput::receiveMessage(CanDataFrame *df) {
    int frameLength = df->getLength();
    if (scheduledDataFrame != NULL) {
        cancelEvent(scheduledDataFrame);
    }
    delete (scheduledDataFrame);
    scheduledDataFrame = df->dup();
    scheduleAt((simTime() + calculateScheduleTiming(frameLength)),
            scheduledDataFrame);
}

void CanPortInput::generateReceiveError(CanDataFrame *df) {
    ErrorFrame *errorMsg = new ErrorFrame("receiveError");
    int pos = intuniform(0, df->getLength() - 12); //Position zwischen 0 - Lï¿½nge des Frames (abzï¿½glich ((EOF und ACK-Delimiter)+1))
    errorMsg->setKind(intuniform(2, 3)); //2: CRC-Fehler, 3: Bit-Stuffing-Fehler
    errorMsg->setCanID(df->getCanID());
    if (pos > 0)
        pos--;  //wegen der verschobenen Sendezeiten
    errorMsg->setPos(pos);
    if (scheduledErrorFrame != NULL) {
        cancelEvent(scheduledErrorFrame);
    }
    delete (scheduledErrorFrame);
    scheduledErrorFrame = errorMsg;
    scheduleAt((simTime() + calculateScheduleTiming(pos)), scheduledErrorFrame);
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
    return ((double) length) / (bandwidth * 1000 * 1000);
//    return ((double) length) / (bandwidth * 1024 * 1024);
}

void CanPortInput::forwardDataFrame(CanDataFrame *df) {
    if (df->getRtr()) {
        for (std::vector<int>::iterator it = outgoingDataFrameIDs.begin();
                it != outgoingDataFrameIDs.end(); ++it) { //TODO muss ich hier noch drauf überprüfen? eigentlich mache ich das glaube ich schon beim empfang des frames
            if (*it == df->getCanID()) {
                emit(rcvdRFSignal, df);
                cModule* sourceApp =
                        getParentModule()->getParentModule()->getSubmodule(
                                "sourceApp");
                sendDirect(df->dup(), sourceApp, "remoteIn");
                break;
            }
        }
    } else {
        for (std::vector<int>::iterator it = incomingDataFrameIDs.begin();
                it != incomingDataFrameIDs.end(); ++it) { //TODO muss ich hier noch drauf überprüfen? eigentlich mache ich das glaube ich schon beim empfang des frames
            if (*it == df->getCanID()) {
                emit(rcvdDFSignal, df);
                send(df->dup(), "out");
                break;
            }
        }
    }
}

void CanPortInput::forwardOwnErrorFrame(ErrorFrame *ef) {
    cModule* portOutput = getParentModule()->getSubmodule("canPortOutput");
    sendDirect(ef->dup(), portOutput, "directIn");
}

void CanPortInput::handleExternErrorFrame(ErrorFrame *ef) {
    CanPortOutput* portOutput =
            (CanPortOutput*) getParentModule()->getSubmodule("canPortOutput");
    portOutput->sendingCompleted();
    if ((checkOutgoingDataFrames(ef->getCanID())
            || checkOutgoingRemoteFrames(ef->getCanID()))) {
        if (ef->getKind() > 2) {
            portOutput->handleReceivedErrorFrame();
        }
        // dieser knoten ist sender; ef an output; da evtl. geschedulte ef lÃ¶schen & neue Arbitrierung
    }
    if (scheduledDataFrame != NULL && scheduledDataFrame->isScheduled()
            && (ef->getCanID() == scheduledDataFrame->getCanID())) {
        cancelEvent(scheduledDataFrame);
    }
    if (scheduledErrorFrame != NULL && scheduledErrorFrame->isScheduled()
            && ef->getCanID() == scheduledErrorFrame->getCanID()) {
        cancelEvent(scheduledErrorFrame);
    }
}
