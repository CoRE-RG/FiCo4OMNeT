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

#include "CanPortOutput.h"

CanPortOutput::CanPortOutput() {
    // TODO Auto-generated constructor stub

}

CanPortOutput::~CanPortOutput() {
    // TODO Auto-generated destructor stub
}

void CanPortOutput::initialize() {
    errors = getParentModule()->getParentModule()->par("errors");
    errorperc = getParentModule()->getParentModule()->par("errorperc");
}

void CanPortOutput::handleMessage(cMessage *msg) {
    send(msg, "out");
    if (errors) {
        int senderr = intuniform(0, 99);
        if (senderr < errorperc) {
            DataFrame *df = check_and_cast<DataFrame *>(msg);
            ErrorFrame *errself = new ErrorFrame("senderror");
            int pos = intuniform(0, df->getLength() - 12); //Position zwischen 0 - L�nge des Frames (abz�glich ((EOF und ACK-Delimiter)+1))
            errself->setKind(intuniform(0, 1)); //0: Bit-Error, 1: Form-Error
//            errself->setNode(vectorid);
            errself->setId(df->getID());
            if (pos > 0)
                pos--;  //wegen der verschobenen Sendezeiten
            errself->setPos(pos);
            send(errself, "out");
        }
    }
}
