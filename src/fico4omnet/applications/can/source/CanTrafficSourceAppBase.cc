//Copyright (c) 2014, CoRE Research Group, Hamburg University of Applied Sciences
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification,
//are permitted provided that the following conditions are met:
//
//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
//3. Neither the name of the copyright holder nor the names of its contributors
//   may be used to endorse or promote products derived from this software without
//   specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
//ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "fico4omnet/applications/can/source/CanTrafficSourceAppBase.h"

#include "fico4omnet/scheduler/can/CanClock.h"
#include "fico4omnet/linklayer/can/CanPortInput.h"

namespace FiCo4OMNeT {

Define_Module(CanTrafficSourceAppBase);

CanTrafficSourceAppBase::CanTrafficSourceAppBase()
{
    this->bitStuffingPercentage = 0;
    this->currentDrift = 0;
}

CanTrafficSourceAppBase::~CanTrafficSourceAppBase()
{
    for (std::list<CanDataFrame*>::iterator it =  outgoingDataFrames.begin(); it != outgoingDataFrames.end(); ++it)
    {
        cancelAndDelete((*it));
    }
    outgoingDataFrames.clear();
}

void CanTrafficSourceAppBase::initialize(int stage) {
    if (stage == 0) {
        canVersion =
                getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->par(
                        "version").stdstringValue();
        bitStuffingPercentage =
                getParentModule()->gate("gate$o")->getPathEndGate()->getOwnerModule()->getParentModule()->par(
                        "bitStuffingPercentage");
        sentDFSignal = registerSignal("txDF");
        sentRFSignal = registerSignal("txRF");
        checkParameterValues();
    } else if (stage == 2) {
        CanClock* canClock =
                dynamic_cast<CanClock*>(getParentModule()->getSubmodule("canClock"));
        currentDrift = canClock->getCurrentDrift();
        initialDataFrameCreation();
        initialRemoteFrameCreation();
    }
}

void CanTrafficSourceAppBase::checkParameterValues() {
    if (bitStuffingPercentage < 0 || bitStuffingPercentage > 1) {
        throw cRuntimeError(
                "The value for the parameter \"bitStuffingPercentage\" is not permitted. Permitted values are from 0 to 1.");
    }
    if (canVersion.compare("2.0A") != 0 && canVersion.compare("2.0B") != 0) {
        throw cRuntimeError(
                "The value for the parameter \"canVersion\" is not permitted. Permitted values are \"2.0B\" and \"2.0A\".");
    }
}

void CanTrafficSourceAppBase::handleMessage(cMessage *msg) {
    CanDataFrame *df = check_and_cast<CanDataFrame *>(msg);
    frameTransmission(df);
}

void CanTrafficSourceAppBase::initialRemoteFrameCreation() {
    if (par("idRemoteFrames").stdstringValue() != "0") {
        cStringTokenizer remoteFrameIDsTokenizer(par("idRemoteFrames"), ",");
        cStringTokenizer remoteFramesPeriodicityTokenizer(par("periodicityRemoteFrames"), ",");
        cStringTokenizer dataLengthRemoteFramesTokenizer(par("dataLengthRemoteFrames"), ",");
        cStringTokenizer initialRemoteFrameOffsetTokenizer(par("initialRemoteFrameOffset"), ",");

        initialFrameCreation("remote", remoteFrameIDsTokenizer, remoteFramesPeriodicityTokenizer,
                dataLengthRemoteFramesTokenizer, initialRemoteFrameOffsetTokenizer);
    }
}

void CanTrafficSourceAppBase::initialDataFrameCreation() {
    if (par("idDataFrames").stdstringValue() != "0") {
        cStringTokenizer dataFrameIDsTokenizer(par("idDataFrames"), ",");
        cStringTokenizer dataFramesPeriodicityTokenizer(par("periodicityDataFrames"), ",");
        cStringTokenizer dataLengthDataFramesTokenizer(par("dataLengthDataFrames"), ",");
        cStringTokenizer initialDataFrameOffsetTokenizer(par("initialDataFrameOffset"), ",");

        initialFrameCreation("data", dataFrameIDsTokenizer, dataFramesPeriodicityTokenizer,
                dataLengthDataFramesTokenizer, initialDataFrameOffsetTokenizer);
    }
}

void CanTrafficSourceAppBase::initialFrameCreation(std::string type,
        cStringTokenizer frameIDsTokenizer, cStringTokenizer framesPeriodicityTokenizer,
        cStringTokenizer dataLengthFramesTokenizer, cStringTokenizer initialFrameOffsetTokenizer) {

    const char *frameType = "";
    if (type.compare("data") == 0) {
        frameType = "message";
    } else {
        frameType = "remoteFrame";
    }
    std::vector<int> frameIDs = frameIDsTokenizer.asIntVector();

    for (unsigned int i = 0; i < frameIDs.size(); i++) {
        if (!dataLengthFramesTokenizer.hasMoreTokens()) {
            throw cRuntimeError("No more values for the %s frame data length for the next %s frame ID. Configuration in the ini file may be incorrect.", type, type);
        }
        if (!framesPeriodicityTokenizer.hasMoreTokens()) {
            throw cRuntimeError("No more values for the %s frame period for the next %s frame ID. Configuration in the ini file may be incorrect.", type, type);
        }
        if (!initialFrameOffsetTokenizer.hasMoreTokens()) {
            throw cRuntimeError("No more values for the %s frame offset for the next %s frame ID. Configuration in the ini file may be incorrect.", type, type);
        }
        CanDataFrame *can_msg = new CanDataFrame(frameType);
        can_msg->setCanID(checkAndReturnID(static_cast<unsigned int> (frameIDs.at(i))));
        unsigned int dataFieldLength = static_cast<unsigned int> (atoi(dataLengthFramesTokenizer.nextToken()));
        can_msg->setBitLength(calculateLength(dataFieldLength));
        cPacket *payload_packet = new cPacket;
        payload_packet->setTimestamp();
        payload_packet->setByteLength(dataFieldLength);
        can_msg->encapsulate(payload_packet);
        can_msg->setPeriod(atof(framesPeriodicityTokenizer.nextToken()));

        if (type.compare("data") == 0) {
            outgoingDataFrames.push_back(can_msg);
            registerDataFrameAtPort(can_msg->getCanID());
        } else {
            can_msg->setRtr(true);
            registerRemoteFrameAtPort(can_msg->getCanID());
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
        if (type.compare("remote") == 0 && can_msg->getPeriod() == 0) {
#pragma GCC diagnostic pop
            EV<< "Remote frame with ID " << can_msg->getCanID()<< " has no period. Hence it will be ignored.\n";
            if (initialFrameOffsetTokenizer.hasMoreTokens()) {
                initialFrameOffsetTokenizer.nextToken();
            }
            delete can_msg;
        } else if (type.compare("remote") == 0 || can_msg->getPeriod() > 0.0) {
            double offset;
            initialFrameOffsetTokenizer.hasMoreTokens() ?
                    offset = atof(initialFrameOffsetTokenizer.nextToken()) : offset = 0;
            simtime_t scheduleTime = simTime() + SimTime(offset)
                    + SimTime(par("periodInaccurracy").doubleValue() + currentDrift);
            if (scheduleTime >= 0 ) {
                scheduleAt(scheduleTime,can_msg);
            } else {
                scheduleAt(simTime(),can_msg);
            }
        } else {
            if (initialFrameOffsetTokenizer.hasMoreTokens()) {
                initialFrameOffsetTokenizer.nextToken();
            }
        }
    }
    if (dataLengthFramesTokenizer.hasMoreTokens()) {
        EV<< "There are more values defined for the " << type << " frame data length. Please check your configuration files.";
    }
    if (framesPeriodicityTokenizer.hasMoreTokens()) {
        EV<< "There are more values defined for the " << type << " frame period. Please check your configuration files.";
    }
    if (initialFrameOffsetTokenizer.hasMoreTokens()) {
        EV<< "There are more values defined for the " << type << " frame offset. Please check your configuration files.";
    }
}

void CanTrafficSourceAppBase::registerRemoteFrameAtPort(unsigned int canID) {
    CanPortInput* port = dynamic_cast<CanPortInput*> (getParentModule()->getSubmodule(
            "canNodePort")->getSubmodule("canPortInput"));
    port->registerOutgoingRemoteFrame(canID);
}

void CanTrafficSourceAppBase::registerDataFrameAtPort(unsigned int canID) {
    CanPortInput* port = dynamic_cast<CanPortInput*> (getParentModule()->getSubmodule(
            "canNodePort")->getSubmodule("canPortInput"));
    port->registerOutgoingDataFrame(canID, this->gate("remoteIn"));
}

unsigned int CanTrafficSourceAppBase::checkAndReturnID(unsigned int canID) {
    if (canVersion.compare("2.0A") == 0) {
        if (canID > VERSIONAMAX) {
            EV<< "ID " << canID << " not valid." << endl;
            endSimulation();
        }
    } else {
        if (canID > VERSIONBMAX) {
            EV << "ID " << canID << " not valid." << endl;
            endSimulation();
        }
    }
    return canID;
}

unsigned int CanTrafficSourceAppBase::calculateLength(unsigned int dataLength) {
    unsigned int arbFieldLength = 0;
    if (canVersion.compare("2.0B") == 0) {
        arbFieldLength += ARBITRATIONFIELD29BIT;
    }
    return (arbFieldLength + DATAFRAMECONTROLBITS
            + calculateStuffingBits(dataLength, arbFieldLength));
}

unsigned int CanTrafficSourceAppBase::calculateStuffingBits(unsigned int dataLength,
        unsigned int arbFieldLength) {
    return static_cast<unsigned int>(((CONTROLBITSFORBITSTUFFING + arbFieldLength + (dataLength * 8) - 1)/ 4) * bitStuffingPercentage);
}

void CanTrafficSourceAppBase::frameTransmission(CanDataFrame *df) {
    CanDataFrame *outgoingFrame = nullptr;

    if (df->getRtr()) {
        emit(sentRFSignal, df);
    } else {
        emit(sentDFSignal, df);
    }

    if (df->isSelfMessage()) {
        outgoingFrame = df->dup();
        CanClock* canClock =
                dynamic_cast<CanClock*>(getParentModule()->getSubmodule("canClock"));
        currentDrift = canClock->getCurrentDrift();
        scheduleAt(
                simTime() + (df->getPeriod())
                        + SimTime(par("periodInaccurracy").doubleValue() + currentDrift), df);
    } else if (df->arrivedOn("remoteIn")) {
        for (std::list<CanDataFrame*>::iterator it =
                outgoingDataFrames.begin(); it != outgoingDataFrames.end();
                ++it) {
            CanDataFrame *tmp = *it;
            if (tmp->getCanID() == df->getCanID()) {
                outgoingFrame = tmp->dup();
                break;
            }
        }
        delete df;
    } else {
        throw cRuntimeError("CanTrafficSourceApp received an invalid message.");
    }

    outgoingFrame->setTimestamp(simTime());
    cPacket* payload_packet = outgoingFrame->decapsulate();
    payload_packet->setTimestamp(simTime());
    outgoingFrame->encapsulate(payload_packet);
    send(outgoingFrame, "out");
}

}
