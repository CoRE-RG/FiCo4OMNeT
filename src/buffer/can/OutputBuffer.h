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

#ifndef OUTPUTBUFFER_H_
#define OUTPUTBUFFER_H_

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "Buffer.h"
#include "CanBusLogic.h"
#include "CanTrafficSourceAppBase.h"
#include "CanPortOutput.h"

using namespace std;

class OutputBuffer: public Buffer {

public:
    virtual void receiveSendingPermission(int id);

    virtual void sendingCompleted(int id);

    /**
     * @brief Puts the frame into the collection and informs the connected gates about the receiption.
     *
     * @param frame The DataFrame to put in the buffer.
     *
     */
    virtual void putFrame(CanDataFrame* frame);

protected:
    virtual void registerForArbitration(int id, bool rtr);

    virtual void checkoutFromArbitration(int id);
};
Define_Module(OutputBuffer);

#endif /* OUTPUTBUFFER_H_ */
