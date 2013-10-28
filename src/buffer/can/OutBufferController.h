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

#ifndef OUTBUFFERCONTROLLER_H_
#define OUTBUFFERCONTROLLER_H_

#include <omnetpp.h>
#include "CanBusApp.h"
#include "Buffer.h"

class OutBufferController : public cSimpleModule {
public:
    virtual void receiveSendingPermission(int id);

    virtual void sendingCompleted(int id);

protected:
    virtual void registerForArbitration(int id, bool rtr, bool remoteSent);



};
Define_Module(OutBufferController);

#endif /* OUTBUFFERCONTROLLER_H_ */
