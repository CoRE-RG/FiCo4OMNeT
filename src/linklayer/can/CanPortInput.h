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

#ifndef CANPORTINPUT_H_
#define CANPORTINPUT_H_

#include <omnetpp.h>
#include <string.h>

using namespace std;

class CanPortInput : public cSimpleModule{
public:
    CanPortInput();
    virtual ~CanPortInput();

protected:
    /**
     *
     */
    virtual void initialize();

    /**
     * @brief Handles all received messages
     *
     * @param msg the incoming message.
     */
    virtual void handleMessage(cMessage *msg);

private:
    /**
     * @brief Vector with relevant incoming messages
     */
    vector<int> incomingIDs;

    /**
     * @brief Sends the the message to the output gate if incomingIDs contains the message ID.
     */
    bool forwardMessage(cMessage *msg);
};

#endif /* CANPORTINPUT_H_ */