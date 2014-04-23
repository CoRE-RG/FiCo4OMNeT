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
    errorReceived = true;
    if (scheduledErrorFrame != NULL && scheduledErrorFrame->isScheduled()) {
        EV<< getParentModule()->getParentModule()->getId() << ": error frame wird gedescheduled\n";
        cancelEvent(scheduledErrorFrame);
        delete scheduledErrorFrame;
        scheduledErrorFrame = NULL;
    }
}

void CanPortOutput::initialize() {
    bandwidth = getParentModule()->getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->par("bandwidth");
    errorsActivated = getParentModule()->getParentModule()->par("errorsActivated");
    errorperc = getParentModule()->getParentModule()->par("errorperc");

    scheduledErrorFrame = new ErrorFrame();

    initializeStatisticValues();
}

//void CanPortOutput::collectStats(){
//
//}

void CanPortOutput::finish(){
//    collectStats();
}

void CanPortOutput::initializeStatisticValues(){
    sentDFSignal = registerSignal("sentDF");
    sentRFSignal = registerSignal("sentRF");
    sendErrorsSignal = registerSignal("sendError");
    receiveErrorsSignal = registerSignal("receiveError");
}

void CanPortOutput::handleMessage(cMessage *msg) {
    std::string msgClass = msg->getClassName();
    if (msgClass.compare("ErrorFrame") == 0) {
        if (!errorReceived) {
            ErrorFrame *ef = check_and_cast<ErrorFrame *>(msg);
            if (ef->getKind() < 2) { //TODO magic number
                emit(sendErrorsSignal, ef);
            } else {
                emit(receiveErrorsSignal, ef);
            }
            colorError();
            send(msg, "out");
            scheduledErrorFrame = NULL;
        } else {
            delete msg;
        }
    } else {
        CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
        colorBusy();
        errorReceived = false;
        if (df->getRtr()) {
            emit(sentRFSignal, df);
        } else {
            emit(sentDFSignal, df);
        }
        send(df, "out");
        if (errorsActivated && (msgClass.compare("CanDataFrame") == 0)) {
            int senderr = intuniform(0, 99);
            if (senderr < errorperc) {
                ErrorFrame *errself = new ErrorFrame("senderror");
                int pos = intuniform(0, df->getLength() - 12); //Position zwischen 0 - L�nge des Frames (abz�glich ((EOF und ACK-Delimiter)+1))
                errself->setKind(intuniform(0, 1)); //0: Bit-Error, 1: Form-Error
                errself->setCanID(df->getCanID());
                if (pos > 0)
                    pos--;  //wegen der verschobenen Sendezeiten
                errself->setPos(pos);
                if (scheduledErrorFrame != NULL && scheduledErrorFrame->isScheduled()) {
                    cancelEvent(scheduledErrorFrame);
                    delete(scheduledErrorFrame);
                }
                scheduledErrorFrame = errself;
                scheduleAt((simTime() + calculateScheduleTiming(pos)),
                        scheduledErrorFrame);
            }
        }
    }
}

double CanPortOutput::calculateScheduleTiming(int length) {
    return ((double) length) / (bandwidth * 1000 * 1000);
//    return ((double) length) / (bandwidth * 1024 * 1024);
}

void CanPortOutput::sendingCompleted(){
    colorIdle();
}

void CanPortOutput::colorBusy(){
    getParentModule()->getParentModule()->getDisplayString().setTagArg("i", 1, "yellow");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 0, "yellow");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 1, "3");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 0, "yellow");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 1, "3");
}

void CanPortOutput::colorIdle(){
    getParentModule()->getParentModule()->getDisplayString().setTagArg("i", 1, "");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 0, "black");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 1, "1");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 0, "black");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 1, "1");
}

void CanPortOutput::colorError(){
    getParentModule()->getParentModule()->getDisplayString().setTagArg("i", 1, "red");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 0, "red");
    getParentModule()->getParentModule()->gate("gate$i")->getDisplayString().setTagArg("ls", 1, "3");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 0, "red");
    getParentModule()->getParentModule()->gate("gate$i")->getPreviousGate()->getDisplayString().setTagArg("ls", 1, "3");
}
