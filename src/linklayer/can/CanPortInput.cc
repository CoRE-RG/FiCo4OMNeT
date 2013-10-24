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

CanPortInput::CanPortInput() {
    // TODO Auto-generated constructor stub

}

CanPortInput::~CanPortInput() {
    // TODO Auto-generated destructor stub
}

void CanPortInput::initialize() {

    cStringTokenizer idIncomingFramesTokenizer(
            getParentModule()->par("idIncomingFrames"), ",");
    incomingIDs = idIncomingFramesTokenizer.asIntVector();

    //TODO stats
//    messagestats.push_back(new Stats(id)); //Statistiken f�r diese Nachricht erstellen
}

void CanPortInput::handleMessage(cMessage *msg) {
    string name = msg->getName();
    if (name.compare("message")) {
        if (!forwardMessage(msg)) {
            EV<<"Message received but not relevant.\n";
            delete msg;
        }
    }
}

bool CanPortInput::forwardMessage(cMessage *msg) {
    for (std::vector<int>::iterator it = incomingIDs.begin();
            it != incomingIDs.end(); ++it) {
        vector<int> temp;
        temp.push_back(msg->getId());
        if (it == temp.end()) {//TODO dirty i think
            send(msg, "out");
            return true;
        }
    }
    return false;
}
