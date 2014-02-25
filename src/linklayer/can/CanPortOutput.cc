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

void CanPortOutput::handleReceivedErrorFrame() {
    EV<< getParentModule()->getParentModule()->getId() << ": error frame wird gedescheduled\n";
    errorReceived = true;
    if (scheduledErrorFrame->isScheduled()) {
        cancelEvent(scheduledErrorFrame);
    }
}

void CanPortOutput::initialize() {
    bandwidth = getParentModule()->getParentModule()->par("bandwidth");
    errors = getParentModule()->getParentModule()->par("errors");
    errorperc = getParentModule()->getParentModule()->par("errorperc");

    scheduledErrorFrame = new ErrorFrame();

    initializeStatisticValues();
}

void CanPortOutput::finish(){
//    collectStats();
}

void CanPortOutput::initializeStatisticValues(){
    sentDFSignal = registerSignal("sentDF");
    sentEFSignal = registerSignal("sentEF");
    sendErrorsSignal = registerSignal("sendError");
    receiveErrorsSignal = registerSignal("receiveError");
//    dataFramesSent = 0;
//    errorFramesSent = 0;
//    WATCH(dataFramesSent);
//    WATCH(errorFramesSent);
}

void CanPortOutput::collectStats(){
//    recordScalar("#sentDF", dataFramesSent);
//    recordScalar("#sentEF", errorFramesSent);
}

void CanPortOutput::handleMessage(cMessage *msg) {
    std::string msgClass = msg->getClassName();
    if (msgClass.compare("ErrorFrame") == 0) {
        if (!errorReceived) {
            ErrorFrame *ef = check_and_cast<ErrorFrame *>(msg);
            emit(sentEFSignal, ef);
//            send(msg->dup(), "out");
            if (ef->getKind() < 2) {
                emit(sendErrorsSignal, ef);
            } else {
                emit(receiveErrorsSignal, ef);
            }
            send(msg, "out");
        }
    } else {
        emit(sentDFSignal, msg);
        send(msg, "out");
        if (errors && (msgClass.compare("CanDataFrame") == 0)) {
            int senderr = intuniform(0, 99);
            if (senderr < errorperc) {
                CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
                ErrorFrame *errself = new ErrorFrame("senderror");
                int pos = intuniform(0, df->getLength() - 12); //Position zwischen 0 - L�nge des Frames (abz�glich ((EOF und ACK-Delimiter)+1))
                errself->setKind(intuniform(0, 1)); //0: Bit-Error, 1: Form-Error
                errself->setCanID(df->getCanID());
                if (pos > 0)
                    pos--;  //wegen der verschobenen Sendezeiten
                errself->setPos(pos);
                cancelEvent(scheduledErrorFrame);
                delete(scheduledErrorFrame);
                scheduledErrorFrame = errself;
                scheduleAt((simTime() + calculateScheduleTiming(pos)),
                        scheduledErrorFrame);
                errorReceived = false;
            }
        }
    }
}

double CanPortOutput::calculateScheduleTiming(int length) {
    return ((double) length) / bandwidth;
}
