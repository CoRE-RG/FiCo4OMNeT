#include "canBusApp.h"

void canBusApp::finish(){
    simtime_t busload = (busytime/simTime())*100;
    if(busload == 0.0 && !idle){
        busload = 100.0;
    }
    recordScalar("#Simulated_Time", simTime());
    recordScalar("%Busload", busload);
    recordScalar("#Data-Frames", numSent);
    recordScalar("#Errors", numErr);
    double errpercentage = (numErr/(double)numSent)*100;
    recordScalar("%Errors", errpercentage);
}

void canBusApp::signIn(cMessage *msg){

    ArbMsg *am = check_and_cast<ArbMsg *>(msg);

    ids.push_back(new ID(am->getId(), am->getNode(), simTime(), am->getRtr(), am->getRemotesent()));
    if(idle){
        cMessage *self = new cMessage("idle_signin");
        ack_rcvd = true;
        scheduleAt(simTime()+(1/(double)bandwidth), self);
        idle = false;
        busytimestamp = simTime();
        char buf[64];
        sprintf(buf, "state: busy");
        bubble("state: busy");
        getDisplayString().setTagArg("tt", 0, buf);
    }
    delete msg;
}

void canBusApp::signOut(cMessage *msg){
    ArbMsg *am = check_and_cast<ArbMsg *>(msg);
    int delid = am->getId();
    bool delrtr = am->getRtr();
    for (list<ID*>::iterator it=ids.begin(); it != ids.end(); ++it){
        ID *id = *it;
        if(id->getId() == delid && id->getRtr() == delrtr){
            it = ids.erase(it);
        }
    }
    EV << "Check-Out fuer die ID " << am->getId() << endl;
    delete msg;
}

void canBusApp::initialize(){
    numSent = 0;
    numErr = 0;
    WATCH(numSent);
    WATCH(numErr);

    busytime = 0.0;
    busytimestamp = 0.0;
    errpos = INT_MAX;
    errored = false;
    stateok = true;
    errorcount = 0;
    ack_rcvd = true;
    idle = true;
    char buf[64];
    sprintf(buf, "state: idle");
    bubble("state: idle");
    getDisplayString().setTagArg("tt", 0, buf);
    payload = getParentModule()->par("payload");
    errors = getParentModule()->par("errors");
    ack = getParentModule()->par("ack");
    bandwidth = getParentModule()->par("bandwidth");

    int i;
    int nodecount = getParentModule()->par("nodecount");
    busPort *port = (busPort*) (getParentModule()->getSubmodule("busPort"));
    for(i = 0; i<nodecount; i++){
        ArbMsg *init = new ArbMsg("initialize");
        init->setNode(i);
        port->sendMsg(init, i);
    }

}

void canBusApp::handleMessage(cMessage *msg){

    busPort *port = (busPort*) (getParentModule()->getSubmodule("port"));

    if(msg->isSelfMessage()){       //Bus ist wieder im Idle-Zustand
        string name = msg->getName();
        //EV << "errorcount: " << errorcount << endl;
        if(name.compare("idle") == 0){  //Wenn zuvor eine Nachricht gesendet wurde
            if(errorcount == 0 || !errors){
                if(ack){
                    if(ack_rcvd){
                        EV << "Nachricht bestaetigt" << endl;
                        ArbMsg *am = check_and_cast<ArbMsg *>(msg);
                        ArbMsg *newam = new ArbMsg("SendingComplete");
                        newam->setNode(am->getNode());
                        newam->setId(am->getId());
                        scheduleAt(simTime()+(11/(double)bandwidth), newam);
                        stateok = false;
                    }else{
                        EV << "Nachricht nicht bestaetigt" << endl;
                        ArbMsg *am = check_and_cast<ArbMsg *>(msg);
                        AckMsg *newam = new AckMsg("SendingFailed");
                        newam->setAck(false);
                        newam->setId(am->getId());
                        port->sendMsg(newam, am->getNode());
                        ArbMsg *sp = new ArbMsg("SendingPermission");
                        sp->setSignInTime(currsit);
                        sp->setId(am->getId());
                        sp->setRemotesent(true);
                        port->sendMsg(sp, am->getNode());
                    }
                }else{
                    ArbMsg *am = check_and_cast<ArbMsg *>(msg);
                    ArbMsg *newam = new ArbMsg("SendingComplete");
                    newam->setId(am->getId());
                    port->sendMsg(newam, am->getNode());
                    stateok = true;
                }
            }else{
                errorcount--;
            }
        }else if(name.compare("SendingComplete") == 0){
            ArbMsg *am = check_and_cast<ArbMsg *>(msg);
            AckMsg *newam = new AckMsg("SendingComplete");
            newam->setId(am->getId());
            stateok = true;
            port->sendMsg(newam, am->getNode());
        }else if(name.compare("ErrResendCompl") == 0){
            ArbMsg *ef = check_and_cast<ArbMsg *>(msg);
            ArbMsg *sp = new ArbMsg("SendingPermission");
            sp->setSignInTime(currsit);
            sp->setRemotesent(true);
            sp->setId(ef->getId());
            vector<list<ID*>::iterator> eraseids;
            for (list<ID*>::iterator it=ids.begin(); it != ids.end(); ++it){
                ID *id = *it;
                if(id->getId() == ef->getId()){
                    eraseids.push_back(it);
                }
            }
            for (unsigned int it = 0; it != eraseids.size(); it++){
                    ids.erase(eraseids.at(it));
            }
            port->sendMsg(sp, ef->getNode());
        }else if(name.compare("ErrResend") == 0){
            ArbMsg *ef = check_and_cast<ArbMsg *>(msg);
            ArbMsg *sp = new ArbMsg("ErrResendCompl");
            sp->setRemotesent(true);
            sp->setId(ef->getId());
            sp->setNode(ef->getNode());
            scheduleAt(simTime()+(errpos/(double)bandwidth), sp);
            errored = false;
            stateok = false;
            errpos = INT_MAX;
        }

        if((!errors || errorcount == 0) && ((ack && ack_rcvd) || !ack) && stateok){
            int highestprio = INT_MAX;
            int hprionode = -1;
            bool remotes = false;
            list<ID*>::iterator delit;
            vector<list<ID*>::iterator> eraseids;
            for (list<ID*>::iterator it=ids.begin(); it != ids.end(); ++it){
                ID *id = *it;
                if(id->getId() < highestprio){
                    highestprio = id->getId();
                    hprionode = id->getNode();
                    remotes = id->getRemotesent();
                    currsit = id->getSignInTime();
                    //EV << "RTR: " << id->getRtr() << endl;
                }
            }
            int sendcount = 0;
            for (list<ID*>::iterator it=ids.begin(); it != ids.end(); ++it){
                ID *id = *it;
                if(id->getId() == highestprio){
                    if(id->getRtr() == false){  //Data-Frame
                        if(hprionode != id->getNode()){
                            hprionode = id->getNode();
                            remotes = id->getRemotesent();
                            currsit = id->getSignInTime();
                            sendcount++;
                        }
                        eraseids.push_back(it);
                    }else{  //Remote-Frame
                        eraseids.push_back(it);
                    }
                }
            }
            if(sendcount > 1){
                EV << "Mehr als ein Knoten will mit der selben ID senden" << endl;
                EV << "Da dies zu fortlaufenden Bit-Fehlern durch die versendenden Knoten fuehrt, wird die Simulation angehalten" << endl;
                endSimulation();
            }

            if(hprionode != -1){
                for (unsigned int it = 0; it != eraseids.size(); it++){
                    ids.erase(eraseids.at(it));
                }
                ArbMsg *am = new ArbMsg("SendingPermission");
                am->setSignInTime(currsit);
                am->setId(highestprio);
                if(remotes){
                    am->setRemotesent(true);
                }else{
                    am->setRemotesent(false);
                }
                port->sendMsg(am, hprionode);  //Erlaubnis verschicken, ID in Liste bereits gel�scht
            }else{
                EV << "no pending message" << endl;
                simtime_t timetaken = simTime() - busytimestamp;
                busytime += timetaken;
                idle = true;
                char buf[64];
                sprintf(buf, "state: idle");
                getDisplayString().setTagArg("tt", 0, buf);
                bubble("state: idle");
            }
        }

        ack_rcvd = true;
        delete msg;
    }else{
        string name = msg->getName();
        if(name.compare("ArbIn") == 0){
            signIn(msg);
        }else if(name.compare("ArbOut") == 0){
            signOut(msg);
        }else if(name.compare("ack") == 0){
            AckMsg *ackm = check_and_cast<AckMsg *>(msg);
            if(ackm->getAck()){
                ack_rcvd = true;
                //EV << "Acknowledged" << endl;
            }
            delete msg;
        }else if(name.compare("senderror") == 0 || name.compare("geterror") == 0){
            ErrorFrame *ef = check_and_cast<ErrorFrame *>(msg);
            //EV << "errored: " << errored << endl;
            if(!errored){
                ArbMsg *amrs = new ArbMsg("ErrResend");
                amrs->setId(ef->getId());
                amrs->setNode(ef->getNode());
                //EV << "errornode: " << ef->getNode() << endl;
                scheduleAt(simTime()+(12/(double)bandwidth), amrs); //12 - maximale L�nge eines Error-Frames
                errorcount++;
                numErr++;
                errored = true;
            }
            if(errpos > ef->getPos()){
                if(errpos < INT_MAX){
                    errpos = ef->getPos();  //Position wird ge�ndert, aber nicht erneut an die Knoten gemeldet
                    //EV << "Neue errpos: " << errpos << endl;
                    delete msg;
                }else{
                    errpos = ef->getPos();
                    //EV << "errpos: " << errpos << endl;
                    port->forward_to_all(msg);
                }
            }else{
                delete msg;
            }

        }else{
            ArbMsg *self = new ArbMsg("idle");
            DataFrame *df = check_and_cast<DataFrame *>(msg);
            self->setNode(df->getGateId());
            self->setId(df->getIdentifier());
            int length = df->getLength();
            //EV << "Length: " << length << endl;
            double nextidle;
            if(ack){
                nextidle = (length-11)/(double)bandwidth; //Bus guckt zum ACK-Slot nach Best�tigung
            }else{
                nextidle = length/(double)bandwidth;
            }
            //EV << "nextidle: " << nextidle << endl;       //Der naechste Idle-Zustand ist eigentlich die (berechnete Zeit - 1), aber hier ist wieder die Sicherheits-Bitzeit mit verrechnet
            scheduleAt(simTime()+nextidle, self);
            ack_rcvd = false;
            numSent++;
            port->forward_to_all(msg);
            //forward_to_all(msg);
        }
    }
}

void canBusApp::forward_to_all(cMessage *msg){
    int i;
    int nodecount = getParentModule()->par("nodecount");
    for(i = 0; i<nodecount; i++){
        //EV << "Sending Copy to node " << i << endl;
        cMessage *copy = (cMessage *) msg->dup();
        send(copy, "gate$o", i);
    }

    delete msg;
}
