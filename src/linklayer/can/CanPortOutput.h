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

#ifndef CANPORTOUTPUT_H_
#define CANPORTOUTPUT_H_

#include <omnetpp.h>
#include "err_m.h"
#include "candataframe_m.h"

class CanPortOutput : public cSimpleModule{
public:
    CanPortOutput();
    virtual ~CanPortOutput();

protected:
    /**
     * @brief Initialization of the module.
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
     * true if errors are activated. Initialized from ned-attribute of CAN-Node
     */
    bool errors;

    /**
    * if errors = true then this is the amount of errors appearing on the node in percent. Initialized from ned-attribute of CAN-Node
    *
    */
    int errorperc;
};
Define_Module(CanPortOutput);
#endif /* CANPORTOUTPUT_H_ */
