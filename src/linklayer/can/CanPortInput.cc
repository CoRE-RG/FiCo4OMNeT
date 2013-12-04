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
    incomingRemoteFrameIDs = idIncomingRemoteFramesTokenizer.asIntVector();

    //TODO stats
//    messagestats.push_back(new Stats(id)); //Statistiken f�r diese Nachricht erstellen
}

void CanPortInput::handleMessage(cMessage *msg) {
    take(msg);
    string name = msg->getName();
    if (name.compare("message") == 0 || name.compare("remoteFrame") == 0) {
        CanDataFrame* df = check_and_cast<CanDataFrame*>(msg);
        if (!forwardMessage(df)) {
            EV<<"Message received but not relevant.\n";
        } else {
            EV<<"Message received and forwarded.\n";
        }
    }
    delete msg;
}

bool CanPortInput::forwardMessage(CanDataFrame *msg) {
    if (msg->getRtr()) {
        for (std::vector<int>::iterator it = incomingRemoteFrameIDs.begin();
                it != incomingRemoteFrameIDs.end(); ++it) {
            CanDataFrame* df = check_and_cast<CanDataFrame*>(msg);
            if (*it == df->getCanID()) {
                cModule* sourceApp =
                        getParentModule()->getParentModule()->getSubmodule(
                                "sourceApp");
                sendDirect(msg->dup(), sourceApp, "remoteIn");
                return true;
            }
        }
    } else {
        for (std::vector<int>::iterator it = incomingDataFrameIDs.begin();
                it != incomingDataFrameIDs.end(); ++it) {
            CanDataFrame* df = check_and_cast<CanDataFrame*>(msg);
            if (*it == df->getCanID()) {
                send(msg->dup(), "out");
                return true;
            }
        }
    }
    return false;
}
