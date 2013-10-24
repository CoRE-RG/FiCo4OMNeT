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

#include "CanNodeApp.h"

/*
 * CONTROL
 */
int canNodeApp::checkUnits(int unitId){
    switch(unitId){
        //1: ID in binary form. Decimal: 431668590 Binary: 11001101110101011110101101110
        case 1: {
                    string bintest = idbinary(431668590);
                    if(bintest.compare("11001101110101011110101101110") == 0){
                        return 0;
                    }
                    break;
                }
        //2: Incoming buffer-test
        case 2: {
                    sendfull = false;
                    getfull = false;
                    numOfMobs = 5;
                    extcode = true;
                    int addm = add_mob(123, 1);
                    if(addm == -1){
                        EV << "Incoming buffer could not be created during unit-test" << endl;
                        return -1;
                    }else{
                        return 0;
                        char testdata[] = "test1234";
                        internalSetBufferData(123, testdata);
                        char *testdata2 = getBufferData(123);
                        bool correct = true;
                        for(int i = 0; i<8; i++){
                            if(testdata[i] != testdata2[i]){
                                correct = false;
                            }
                        }
                        if(correct){
                            return 0;
                        }else{
                            return -1;
                        }
                    }
                    break;
                }
        //3: Outgoing buffer-test
        case 3: {
                    sendfull = false;
                    getfull = false;
                    numOfMobs = 5;
                    extcode = true;
                    int addm = add_mob(123, 0);
                    if(addm == -1){
                        EV << "Outgoing buffer could not be created during unit-test" << endl;
                        return -1;
                    }else{
                        char testdata[] = "test1234";
                        setBufferData(123, testdata);
                        char *testdata2 = internalGetBufferData(123);
                        bool correct = true;
                        for(int i = 0; i<8; i++){
                            if(testdata[i] != testdata2[i]){
                                correct = false;
                            }
                        }
                        if(correct){
                            return 0;
                        }else{
                            return -1;
                        }
                    }
                    break;
                }
        //4: Stats collection test
        case 4: {
                    Stats *testStats = new Stats(1234);
                    simtime_t testtime = simTime();
                    testStats->newTime(testtime+0.001);
                    testStats->newTime(testtime+0.01);
                    testStats->newTime(testtime+0.0025);
                    testStats->newTime(testtime+0.003);
                    testStats->newTime(testtime+0.0015);
                    testStats->newTime(testtime+0.001);
                    testStats->newTime(testtime+0.00125);
                    testStats->newTime(testtime+0.005);
                    testStats->newTime(testtime+0.006);
                    testStats->newTime(testtime+0.00875);
                    simtime_t testmin = testStats->getMin();
                    simtime_t testmax = testStats->getMax();
                    simtime_t testavg = testStats->getMean();
                    bool correct = true;
                    if(testmin != 0.001 || testmax != 0.01 || testavg != 0.004) correct = false;
                    if(correct){
                        return 0;
                    }else{
                        return -1;
                    }
                    break;
                }
        //5: Original Bit Stuffing test
        case 5: {
                    char testdata[] = "<<<<<<<<";   //Verursacht viele stuffing bits
                    string idbintest = idbinary(120);
                    MOB *testmob = new MOB(0, 120, 50, testdata, 3, 0, true, idbintest);
                    int testlength = testmob->getLength();
                    bool correct = true;
                    if(testlength != 130) correct = false;
                    if(correct){
                        return 0;
                    }else{
                        return -1;
                    }
                    break;
                }

    }
    return -2;
}

void canNodeApp::initialize(){
    /*EV << "test1: " << ((checkUnits(1)==0)?(" success"): ("no success")) << endl;
    EV << "test2: " << ((checkUnits(2)==0)?(" success"): ("no success")) << endl;
    EV << "test3: " << ((checkUnits(3)==0)?(" success"): ("no success")) << endl;
    EV << "test4: " << ((checkUnits(4)==0)?(" success"): ("no success")) << endl;
    EV << "test5: " << ((checkUnits(5)==0)?(" success"): ("no success")) << endl;*/

    numSent = 0;
    rcvdInteresting = 0;
    WATCH(numSent);
    WATCH(rcvdInteresting);
    sendfull = false;
    getfull = false;
    numOfMobs = getParentModule()->par("numOfMobs");
    errocc = 0;
    extcode = getParentModule()->par("extcode");
    percentage = getParentModule()->par("percentage");
    bitstuffing = getParentModule()->par("bitstuffing");
    errorperc = getParentModule()->par("errorperc");
    string ver = getParentModule()->par("version");
    if(ver.compare("2.0A") != 0 && ver.compare("2.0B") != 0){
        EV << "Keine gueltige Einstellung fuer die Version. Gueltig: 2.0A oder 2.0B. Angegebener Wert: " << version << endl;
        endSimulation();
    }
    if(ver.compare("2.0A") == 0){
        version = true;
    }else{
        version = false;
    }
    if(bitstuffing < 0 || bitstuffing > 3){
        EV << "Keine gueltige Einstellung fuer Bitstuffing. Gueltiger Bereich: 0-3. Angegebener Wert: " << bitstuffing << endl;
        endSimulation();
    }
    if(bitstuffing == 2 && (percentage < 0 || percentage > 100)){
        EV << "Keine gueltige Einstellung fuer prozentuale Verteilung. Gueltiger Bereich: 0-100. Angegebener Wert: " << percentage << endl;
        endSimulation();
    }
    if(errorperc < 0 || errorperc > 100){
        EV << "Keine gueltige Einstellung fuer Fehlerh�ufigkeit. Gueltiger Bereich: 0-100. Angegebener Wert: " << errorperc << endl;
        endSimulation();
    }
    payload = getParentModule()->par("payload");
    errors = getParentModule()->par("errors");
    ack = getParentModule()->par("ack");
    bandwidth = getParentModule()->par("bandwidth");
    if(bandwidth < 0 || bandwidth > 1000000){
        EV << "Ungueltige Bandbreiteneinstellung" << endl;
        endSimulation();
    }
//    string node = getParentModule()->par("node");
    char buf[64];
    sprintf(buf,getParentModule()->par("node"));
    getDisplayString().setTagArg("t", 0, buf);
    bool trafficGen = getParentModule()->par("trafficGen");
    if(trafficGen){
        trafficGenInit();
    }
}

void canNodeApp::finish(){
    collectStats();
}

void canNodeApp::collectStats(){
    recordScalar("#sent", numSent);
    rcvdInteresting -= numSent;
    if(rcvdInteresting < 0) rcvdInteresting = 0;
    recordScalar("#interesting_received_messages", rcvdInteresting);
    string node = getParentModule()->par("node");
    for(unsigned int i = 0; i<messagestats.size(); i++){
        char buf[64] = {0};
        sprintf(buf, "%d", messagestats.at(i)->getId());
        string idstr = string(buf);
        string min = idstr + "_min(in_ms)";
        string max = idstr + "_max(in_ms)";
        string mean = idstr + "_avg(in_ms)";
        string jitter = idstr + "_jitter(ms)";
        for(unsigned int j = 0; j<min.size(); j++){
            buf[j] = min[j];
        }
        recordScalar(buf, messagestats.at(i)->getMin());
        memset((void*)buf, 0, 64);
        for(unsigned int j = 0; j<min.size(); j++){
            buf[j] = jitter[j];
        }
        simtime_t jit = messagestats.at(i)->getMax() - messagestats.at(i)->getMin();
        recordScalar(buf, jit);
        memset((void*)buf, 0, 64);
        for(unsigned int j = 0; j<min.size(); j++){
            buf[j] = max[j];
        }
        recordScalar(buf, messagestats.at(i)->getMax());

        memset((void*)buf, 0, 64);
        for(unsigned int j = 0; j<min.size(); j++){
            buf[j] = mean[j];
        }
        recordScalar(buf, messagestats.at(i)->getMean());
    }
}
/*
 * BUFFER
 */
char* canNodeApp::getBufferData(int id){
    int bufferidx = 0;
    int wantedbufferidx = -1;
    for (vector<Buffer*>::iterator it=phys_mobs.begin(); it != phys_mobs.end(); ++it){
        Buffer *tmp = *it;
        if((tmp->getId() == id) && (tmp->getMode() == 1)){  //Spezielle Empfangserlaubnis
            wantedbufferidx = bufferidx;
            break;
        }
        if((tmp->getId() == 0) && (tmp->getMode() == 1)){   //Allgemeine Empfangserlaubnis
            wantedbufferidx = bufferidx;
            break;
        }
        bufferidx++;
    }
    if(wantedbufferidx != -1){
        char *data = phys_mobs.at(wantedbufferidx)->getData();
        return data;
    }else{
        EV << "getBufferData failure" << endl;
        char ret[3];
        sprintf(ret, "%d", -1);
        char *retpointer = ret;
        return retpointer;
    }
}

int canNodeApp::internalSetBufferData(int id, char datain[]){
    //Diese Methode schreibt bei Empfang von Nachrichten auf den entsprechenden Empfangspuffer
    int bufferidx = 0;
    int wantedbufferidx = -1;
    for (vector<Buffer*>::iterator it=phys_mobs.begin(); it != phys_mobs.end(); ++it){
        Buffer *tmp = *it;
        if((tmp->getId() == id) && (tmp->getMode() == 1)){  //Spezielle Empfangserlaubnis
            wantedbufferidx = bufferidx;
            break;
        }
        if((tmp->getId() == 0) && (tmp->getMode() == 1)){   //Allgemeine Empfangserlaubnis
            wantedbufferidx = bufferidx;
            break;
        }
        bufferidx++;
    }

    if(wantedbufferidx != -1){
    //Daten in den Puffer schreiben
        char data[8] = {0};
        for(int i = 0; i<8; i++){
            data[i] = datain[i];
        }
        phys_mobs.at(wantedbufferidx)->setData(data);
        return 0;
    }else{
        return -1;
        //EV << "ID nicht gefunden" << endl;
    }
}

char* canNodeApp::internalGetBufferData(int id){
    int bufferidx = 0;
    int wantedbufferidx = -1;
    for (vector<Buffer*>::iterator it=phys_mobs.begin(); it != phys_mobs.end(); ++it){
        Buffer *tmp = *it;
        if((tmp->getId() == id) && (tmp->getMode() == 0)){  //Spezielle Sendeerlaubnis
            wantedbufferidx = bufferidx;
            break;
        }
        if((tmp->getId() == 0) && (tmp->getMode() == 0)){   //Allgemeine Sendeerlaubnis
            wantedbufferidx = bufferidx;
            break;
        }
        bufferidx++;
    }
    if(wantedbufferidx != -1){
        char *data = phys_mobs.at(wantedbufferidx)->getData();
        return data;
    }else{
        EV << "internalGetBufferData failure" << endl;
        char ret[3];
        sprintf(ret, "%d", -1);
        char *retpointer = ret;
        return retpointer;
    }
}

int canNodeApp::setBufferData(int id, char datain[]){
    int bufferidx = 0;
    int wantedbufferidx = -1;
    for (vector<Buffer*>::iterator it=phys_mobs.begin(); it != phys_mobs.end(); ++it){
        Buffer *tmp = *it;
        if((tmp->getId() == id) && (tmp->getMode() == 0)){  //Spezielle Sendeerlaubnis
            wantedbufferidx = bufferidx;
            break;
        }
        if((tmp->getId() == 0) && (tmp->getMode() == 0)){   //Allgemeine Sendeerlaubnis
            wantedbufferidx = bufferidx;
            break;
        }
        bufferidx++;
    }

    if(wantedbufferidx != -1){
    //Daten in den Puffer schreiben
        char data[8] = {0};
        for(int i = 0; i<8; i++){
            data[i] = datain[i];
        }
        phys_mobs.at(wantedbufferidx)->setData(data);
        return 0;
    }else{
        return -1;
        //EV << "ID nicht gefunden" << endl;
    }
}
/*
 * HANDLE MESSAGES
 */
void canNodeApp::handleMessage(cMessage *msg){
    if(msg->isSelfMessage()){
        string name = msg->getName();
        if(name.compare("remote") == 0){    //Remote-Anfrage
            remoteRequest(msg);
        }else if(name.compare("ack") == 0){ //Bei keinem Fehler ACK abschicken und Sendezeit berechnen
            if(errocc == 0){
                AckMsg *am = check_and_cast<AckMsg *>(msg);
                simtime_t timespent = simTime() - currst + (1/(double)bandwidth);
                timespent = ((timespent*bandwidth)/1000);
                int id = am->getId();
                newTime(id, timespent);
//                send(msg, "gate$o");
                nodePort *port = (nodePort*) (getParentModule()->getSubmodule("nodePort"));
                port->sendMsgToBus(msg);
            }else{
                delete msg;
                errocc--;
            }
        }else if(name.compare("timespent") == 0){
            if(errocc == 0){
                ArbMsg *am = check_and_cast<ArbMsg *>(msg);
                simtime_t timespent = simTime() - currst + (1/(double)bandwidth);
                timespent = ((timespent*bandwidth)/1000);
                int id = am->getId();
                newTime(id, timespent);
            }else{
                errocc--;
            }
            delete msg;
        }else if(name.compare("timespent1") == 0){
            if(errocc != 0){
                errocc--;
            }
            delete msg;
        }else{      //Nachricht soll nach abgelaufener Periode auf den Bus gelegt werden
            char cidx[64];
            sprintf(cidx, "%s", msg->getName());
            int index = atoi(cidx);
            MOB *mob = out_m.at(index);
            ArbMsg *am = new ArbMsg("ArbIn");
            cMessage *self = new cMessage(msg->getName());
            double nextsend = mob->getPeriod()/(double)1000;
            scheduleAt(simTime()+nextsend, self);
            am->setNode(vectorid);
            am->setId(mob->getID());
            am->setRemotesent(false);

            if(mob->getMode() == 1){    //RTR-Bit = 1
                am->setRtr(true);
            }else{
                am->setRtr(false);
            }
            nodePort *port = (nodePort*) (getParentModule()->getSubmodule("nodePort"));
            port->sendMsgToBus(am);
//            send(am, "gate$o");
            delete msg;
        }
    }else{
        string name = msg->getName();
        if(name.compare("SendingPermission") == 0){     //Knoten darf senden
            cMessage *msgcopy = msg->dup();
            sendDFrame(msgcopy);
        }else if(name.compare("SendingComplete") == 0){ //Nachricht mit enthaltener ID vollst�ndig verschickt
            if(ack){
                AckMsg *ackrec = check_and_cast<AckMsg *>(msg);
                EV << "Nachricht mit ID " << ackrec->getId() << " vollst�ndig verschickt" << endl;
            }else{
                ArbMsg *am = check_and_cast<ArbMsg *>(msg);
                EV << "Nachricht mit ID " << am->getId() << " vollst�ndig verschickt" << endl;
            }
        }else if(name.compare("initialize") == 0){
            ArbMsg *am = check_and_cast<ArbMsg *>(msg);
            vectorid = am->getNode();
//            port = (nodePort*) (getParentModule()->getSubmodule("nodePort"));
        }else if(name.compare("SendingFailed") == 0){
            if(ack){
                AckMsg *ackrec = check_and_cast<AckMsg *>(msg);
                EV << "Nachricht mit ID " << ackrec->getId() << " nicht korrekt verschickt" << endl;
            }else{
                ArbMsg *am = check_and_cast<ArbMsg *>(msg);
                EV << "Nachricht mit ID " << am->getId() << " nicht korrekt verschickt" << endl;
            }
        }else if(name.compare("senderror") == 0 || name.compare("geterror") == 0){
            EV << "Fehler erkannt" << endl;
            errocc++;
        }else{                      //Angekommener Daten-Frame
            cMessage *messagecopy = msg->dup();
            handleDFrame(messagecopy);
        }
        delete msg;
    }
}

void canNodeApp::remoteRequest(cMessage *msg){
    /*
     * Called when a remote-frame is recognized by the node
     *
     * The node will search for the desired message to schedule it at immediately for sending
     *
     */
    ArbMsg *amin = check_and_cast<ArbMsg *>(msg);
    int mobId = amin->getId();
    int wantedidx = -1;
    int idx = 0;
    for (vector<MOB*>::iterator it=out_m.begin(); it != out_m.end(); ++it){
        MOB *id = *it;
        if(id->getID() == mobId){
            wantedidx = idx;
        }
        idx++;
    }
    if(wantedidx != -1){
        MOB *mob = out_m.at(wantedidx);
        if(mob->getMode() == 0){    //Nachricht zum Senden
            EV << "Nachricht mit ID " << mob->getID() << " wird versendet" << endl;
            bool allowed = false;
            for (vector<Buffer*>::iterator it=phys_mobs.begin(); it != phys_mobs.end(); ++it){
                Buffer *tmp = *it;
                if((tmp->getId() == mob->getID()) && (tmp->getMode() == 0)){    //Spezielle Sendeerlaubnis
                    allowed = true;
                }
                if((tmp->getId() == 0) && (tmp->getMode() == 0)){               //Allgemeine Sendeerlaubnis
                    allowed = true;
                }
            }
            if(allowed){
                ArbMsg *am = new ArbMsg("ArbIn");
                am->setNode(vectorid);
                am->setId(mob->getID());
                am->setRtr(false);
                am->setRemotesent(true);
                nodePort *port = (nodePort*) (getParentModule()->getSubmodule("nodePort"));
                port->sendMsgToBus(am);
//                send(am, "gate$o");
            }else{
                EV << "Diese Nachricht darf nicht verschickt werden (Kein Puffer daf�r registriert)" << endl;
            }
        }
    }else{
        //EV << "Keine Nachricht dieser ID von diesem Knoten zum Senden" << endl;
    }
    delete msg;
}

void canNodeApp::handleDFrame(cMessage *msg){
    /*
     * An incoming data-frame is processed through this method
     *
     * The method will parse the message to an Data-Frame-Object and takes actions according to the omnetpp.ini-configuration
     *
     */
    EV << "Nachricht angekommen" << endl;
    DataFrame *df = check_and_cast<DataFrame *>(msg);
    EV << "Identifier = " << df->getIdentifier() << "." << endl;
    currst = df->getStartTime();
    bool wanted = false;
    if(errors){
        string node = getParentModule()->par("node");
        if(node.compare(df->getNode()) != 0){
            int geterr = intuniform(0,99);
            //EV << "geterr: " << geterr << endl;
            if(geterr < errorperc){
                ErrorFrame *errself = new ErrorFrame("geterror");
                int pos = intuniform(0,df->getLength()-12); //Position zwischen 0 - L�nge des Frames (abz�glich ((EOF und ACK-Delimiter)+1))
                //EV << "pos: " << pos << endl;
                errself->setKind(intuniform(2,3));      //2: CRC-Fehler, 3: Bit-Stuffing-Fehler
                errself->setNode(df->getGateId());
                errself->setId(df->getIdentifier());
                if(pos > 0) pos--;  //wegen der verschobenen Sendezeiten
                errself->setPos(pos);
                nodePort *port = (nodePort*) (getParentModule()->getSubmodule("nodePort"));
                port->sendMsgToBus(errself);
//                send(errself, "gate$o");
                //scheduleAt(simTime()+((pos)/(double)bandwidth), errself);
            }
        }
    }

    //ACK:
    if(ack){
        string node = getParentModule()->par("node");
        if(node.compare(df->getNode()) != 0){
            AckMsg *ackm = new AckMsg("ack");
            ackm->setId(df->getIdentifier());
            if(!errors){
                ackm->setAck(true);
            }else{      //Fehler aktiviert
                int err = intuniform(0,100);
                if(err <= errorperc){
                    ackm->setAck(false);
                }else{
                    ackm->setAck(true);
                }
            }
            scheduleAt(simTime()+((df->getLength()-12)/(double)bandwidth), ackm);   //Wird eine Bitzeit vor dem ACK-Slot versandt, damit Bus zu ACK-Slot pr�fen kann
        }else{
            ArbMsg *self = new ArbMsg("timespent1");
            self->setId(df->getIdentifier());
            scheduleAt(simTime()+((df->getLength()-12)/(double)bandwidth), self);
        }
    }else{
        ArbMsg *self = new ArbMsg("timespent");
        self->setId(df->getIdentifier());
        scheduleAt(simTime()+((df->getLength()-12)/(double)bandwidth), self);
    }

    bool getallowed = false;
    for (vector<Buffer*>::iterator it=phys_mobs.begin(); it != phys_mobs.end(); ++it){
        Buffer *tmp = *it;
        if((tmp->getId() == df->getIdentifier()) && (tmp->getMode() == 1)){ //Spezielle Empfangserlaubnis
            getallowed = true;
            break;
        }
        if((tmp->getId() == 0) && (tmp->getMode() == 1)){               //Allgemeine Empfangserlaubnis
            getallowed = true;
            break;
        }
    }
    if(getallowed){
        wanted = true;
    }else{
        EV << "Diese Nachricht wird durch den Knoten nicht verarbeitet" << endl;
    }

    if(wanted){
        rcvdInteresting++;
        //EV << "Die Nachricht verarbeite ich." << endl;

        if(df->getRtr() == 1){  //Remote-Frame
            ArbMsg *self = new ArbMsg("remote");
            self->setId(df->getIdentifier());
            scheduleAt(simTime(), self);
            EV << "Remote-Frame" << endl;
        }
        //Bei aktiviertem Payload:
        else if(payload){
            char data[8];
            for(int i = 0; i<8; i++){
                data[i] = df->getData(i);
            }
            internalSetBufferData(df->getIdentifier(), data);

            EV << "Daten: " << getBufferData(df->getIdentifier()) << endl;
        }
    }
    delete msg;
}

void canNodeApp::newTime(int id, simtime_t timetaken){
    /*
     * Called for statistics collection
     *
     * If a message was received, this method will ad its sending time to the associated Stats-Object
     *
     */
    int wantedidx = -1;
    int idx = 0;
    for (vector<Stats*>::iterator it=messagestats.begin(); it != messagestats.end(); ++it){
        Stats *statstemp = *it;
        if(statstemp->getId() == id){
             wantedidx = idx;
             break; //Fertig, weitere Iteration unn�tig
        }
        idx++;
    }

    if(wantedidx != -1){
        messagestats.at(wantedidx)->newTime(timetaken);
        EV << "Zeit für die Versendung von ID " << id << ": " << timetaken << " ms" << endl;
    }/*else{
        EV << "Statistiken f�r " << id << " nicht gefunden" << endl;
    }*/

}
/*
 * SEND FRAME
 */
void canNodeApp::sendDFrame(cMessage *msg){
    /*
     * The node sends a data-frame to the bus upon getting the permission
     *
     * The list out_m is used to generate the specific data-frame.
     * Afterwards, the message will be re-scheduled.
     *
     */
    ArbMsg *am = check_and_cast<ArbMsg *>(msg);
    int mobId = am->getId();
    EV << "Send Permission achieved for mobId " << mobId << endl;
    int wantedidx = -1;
    int idx = 0;
    for (vector<MOB*>::iterator it=out_m.begin(); it != out_m.end(); ++it){
        MOB *id = *it;
        if(id->getID() == mobId){
            wantedidx = idx;
        }
        idx++;
    }

    if(wantedidx != -1){    //Data-Frame auf den Bus legen
        MOB *mob = out_m.at(wantedidx);
        DataFrame *can_msg = new DataFrame("message");

        can_msg->setNode(getParentModule()->par("node"));
        can_msg->setIdentifier(mob->getID());
        can_msg->setLength(mob->getLength());
        can_msg->setStartTime(am->getSignInTime());
        if(payload){
            char* data = internalGetBufferData(mob->getID());
            for(int i = 0; i<8; i++){
                can_msg->setData(i, data[i]);
            }
        }
        if(mob->getMode() == 0){
            can_msg->setRtr(false);
        }else{
            can_msg->setRtr(true);
        }
        can_msg->setGateId(vectorid);

        /*if(!(am->getRemotesent())){
            //Nachricht nach abgelaufener Periode wieder verschicken:
            char index[64];
            sprintf(index, "%d", wantedidx);
            cMessage *self = new cMessage(index);
            double nextsend = mob->getPeriod()/(double)1000;
            scheduleAt(simTime()+nextsend-(1/(double)bandwidth), self);
        }*/
//        send(can_msg, "gate$o");        //DataFrame abschicken
        nodePort *port = (nodePort*) (getParentModule()->getSubmodule("nodePort"));
        port->sendMsgToBus(can_msg);
        if (errors) {
            int senderr = intuniform(0, 99);
            //EV << "senderr: " << senderr << endl;
            if (senderr < errorperc) {
                ErrorFrame *errself = new ErrorFrame("senderror");
                int pos = intuniform(0, mob->getLength() - 12); //Position zwischen 0 - L�nge des Frames (abz�glich ((EOF und ACK-Delimiter)+1))
                //EV << "pos: " << pos << endl;
                errself->setKind(intuniform(0, 1)); //0: Bit-Error, 1: Form-Error
                errself->setNode(vectorid);
                errself->setId(mob->getID());
                if (pos > 0)
                    pos--;  //wegen der verschobenen Sendezeiten
                errself->setPos(pos);
//                send(errself, "gate$o");
//                nodePort *port = (nodePort*) (getParentModule()->getSubmodule(
//                        "nodePort"));
                port->sendMsgToBus(errself);
                //scheduleAt(simTime()+((pos)/(double)bandwidth), errself);
            }
        }
        numSent++;
    }else{
        EV << "Keine Nachricht mit dieser ID in diesem Knoten zum Senden" << endl;
    }
    delete msg;
}
/*
 * MOB
 */
int canNodeApp::add_mob(int id, int mode){

    if(mode < 0 || mode > 1){
        //Nix tun, ung�ltiger Wert
        EV << "Ungültiger Modus für Hinzufügen eines Message-Puffers" << endl;
        return -1;
    }else{
        if(extcode){
            if(phys_mobs.size() < numOfMobs){
                phys_mobs.push_back(new Buffer(id, mode));
                return 0;
            }else{
                EV << "Kein freier Slot, erst Buffer manuell frei machen" << endl;
                return -1;
            }
            return -1;
        }else{
            if(phys_mobs.size() < numOfMobs){
                if((mode == 0 && !sendfull) || (mode == 1 && !getfull)){
                    phys_mobs.push_back(new Buffer(id, mode));
                    return 0;
                }else{
                    EV << "Puffer bereits auf allgmeine Gültigkeit gestellt" << endl;
                    EV << phys_mobs.size() << endl;
                    return 1;
                }
            }else{
                EV << "Max. Größe MOBs erreicht.";
                if(mode == 1){
                    EV << " Empfangspuffer empfangen nun alle Nachrichten" << endl;
                }else{
                    EV << " Sendepuffer versenden nun alle Nachrichten" << endl;
                }
                if((!sendfull && mode == 0) || (!getfull && mode == 1)){
                    int count = 0;
                    list<vector<Buffer*>::iterator> delits;
                    Buffer *tmp;
                    //L�schen der anderen IDs und umstellen der Puffer auf "Alle senden/empfangen":
                    for (vector<Buffer*>::iterator it=phys_mobs.begin(); it != phys_mobs.end(); ++it){
                        tmp = *it;
                        if(tmp->getMode() == mode){
                            delits.push_back(it);
                            count++;
                        }
                    }
                    for (list<vector<Buffer*>::iterator>::iterator delit=delits.begin(); delit != delits.end(); ++delit){
                        vector<Buffer*>::iterator del = *delit;
                        phys_mobs.erase(del);
                    }
                    if(count == 0){
                        if(mode == 1){
                            EV << "Kein Empfangspuffer zum �berschreiben" << endl;
                        }else{  //mode = 0
                            EV << "Kein Sendepuffer zum �berschreiben" << endl;
                        }
                        return -1;
                    }else{
                        phys_mobs.push_back(new Buffer(0, mode));   //Allgemeine Sende-/Empfangserlaubnis
                        if(mode == 1){
                            getfull = true;
                        }else{  //mode == 0
                            sendfull = true;
                        }
                        return 1;
                    }
                }else{
                    return 1;
                }
            }
        }
    }
}

int canNodeApp::remove_mob(int id){
    bool success = false;
    for (vector<Buffer*>::iterator it=phys_mobs.begin(); it != phys_mobs.end(); ++it){
        Buffer *tmp = *it;
        if(tmp->getId() == id){
            it = phys_mobs.erase(it);
            success = true;
        }
    }
    if(success){
        return 0;
    }else{
        return -1;
    }
}
/*
 * TRAFFIC GEN
 */
void canNodeApp::trafficGenInit(){
    /*
    * Initializes the omnetpp.ini-traffic-generator
    *
    * If the traffic generator is enabled, this method initializes internal lists of the node.
    * When a message is scheduled, the node uses the information of those internal lists for the data and a re-schedule.
    *
    */
    string node = getParentModule()->par("node");

    const char *id = getParentModule()->par("id");
    cStringTokenizer idtkn(id);
    idtkn.setDelimiter(",");

//    const char *period = getParentModule()->par("periodicity");
//    cStringTokenizer prtkn(period);
    cStringTokenizer prtkn(getParentModule()->par("periodicity"), ",");
//    prtkn.setDelimiter(",");

    const char *datalength = getParentModule()->par("datalength");
    cStringTokenizer dltkn(datalength);
    dltkn.setDelimiter(",");

    const char *mode = getParentModule()->par("mode");
    cStringTokenizer mdtkn(mode);
    mdtkn.setDelimiter(",");

    const char *data = getParentModule()->par("data");
    cStringTokenizer dttkn(data);

    //Einf�gen der Nachrichten in die Liste des Knotens:
    int nom = getParentModule()->par("numOfMessages");
    for(int i = 0; i < nom; i++){
        if(idtkn.hasMoreTokens() && prtkn.hasMoreTokens() && (dltkn.hasMoreTokens() || dttkn.hasMoreTokens()) && mdtkn.hasMoreTokens()){
            int id = atoi(idtkn.nextToken());
            if(version){            //2.0A
                if(id < 0 || id > 2047){
                    EV << "ID " << id << " ist nicht gueltig." << endl;
                    endSimulation();
                }
            }else{                  //2.0B
                if(id < 0 || id > 536870911){
                    EV << "ID " << id << " ist nicht gueltig." << endl;
                    endSimulation();
                }
            }

            //Modus (Data-Frame, Remote-Frame, Empfangener Frame)
            int mode = atoi(mdtkn.nextToken());
            if(mode < 0 || mode > 2){
                EV << "Modus " << mode << " ist nicht gueltig. Zahl von 0 bis 2 wählen." << endl;
                endSimulation();
            }
            if(bitstuffing != 3 || mode == 2){  //Alle Bitstuffing sequenzen au�er original - und bei Empfangen von Nachrichten
                if(mode == 2){  //Empfangsnachricht
                    messagestats.push_back(new Stats(id));  //Statistiken f�r diese Nachricht erstellen
                    in_m.push_back(id);
                    add_mob(id, 1);     //Nachricht in physikalischen Puffer schreiben
                    EV << "Empfangsnachricht " << id << " eingetragen" << endl;
                    if(payload && dttkn.hasMoreTokens()){
                        dttkn.nextToken();
                    }else if(!payload && dltkn.hasMoreTokens()){
                        dltkn.nextToken();
                    }else{
                        EV << "Keine Dummy-Werte f�r Empfangsnachricht angegeben" << endl;
                        endSimulation();
                    }
                    prtkn.nextToken();
                }else{          //normale Nachricht
                    if(payload && dttkn.hasMoreTokens()){
                        char datachar[64] = {0};
                        sprintf(datachar, "%s", dttkn.nextToken());
                        int size = 0;
                        for(int i = 0; i<64; i++){
                            if(datachar[i] == 0){
                                break;
                            }else{
                                size++;
                            }
                        }
                        if(size > 8){
                            EV << "Daten sind zu Lang. Maximale L�nge ist 8 Byte, diese Daten sind " << size << " Byte lang" << endl;
                            endSimulation();
                        }
                        add_mob(id,0);
                        add_mob(id,1);      //F�r Remote-Frame-Anfragen
                        out_m.push_back(new MOB(mode, id, atoi(prtkn.nextToken()), datachar, bitstuffing, percentage, version));    //MOB-Konstruktor 3
                        if(mode == 0){
                            setBufferData(id, datachar);
                        }
                    }else{
                        if(dltkn.hasMoreTokens()){
                            add_mob(id,0);
                            add_mob(id,1);      //F�r Remote-Frame-Anfragen
                            out_m.push_back(new MOB(mode, id, atoi(prtkn.nextToken()), atoi(dltkn.nextToken()), bitstuffing, percentage, version)); //MOB-Konstruktor 1
                        }else{
                            EV << "Keine Datenl�ngen mehr vorhanden" << endl;
                            endSimulation();
                        }
                    }
                }
            }else{                  //Original Bit stuffing
                string idbinar = idbinary(id);
                if(payload && dttkn.hasMoreTokens()){
                    char datachar[64] = {0};
                    sprintf(datachar, "%s", dttkn.nextToken());
                    int size = 0;
                    for(int i = 0; i<64; i++){
                        if(datachar[i] == 0){
                            break;
                        }else{
                            size++;
                        }
                    }
                    if(size > 8){
                        EV << "Daten sind zu Lang. Maximale L�nge ist 8 Byte, diese Daten sind " << size << " Byte lang" << endl;
                        endSimulation();
                    }
                    add_mob(id,0);
                    add_mob(id,1);      //F�r Remote-Frame-Anfragen
                    out_m.push_back(new MOB(mode, id, atoi(prtkn.nextToken()), datachar, bitstuffing, percentage, version, idbinar));   //MOB-Konstruktor 4
                    if(mode == 0){
                        setBufferData(id, datachar);
                    }
                }else{
                    if(dltkn.hasMoreTokens()){
                        add_mob(id,0);
                        add_mob(id,1);      //F�r Remote-Frame-Anfragen
                        out_m.push_back(new MOB(mode, id, atoi(prtkn.nextToken()), atoi(dltkn.nextToken()), bitstuffing, percentage, version, idbinar)); //MOB-Konstruktor 2
                    }else{
                        EV << "Nicht genug Daten oder Datenl�ngen f�r Node " << node << " angegeben. Unter Canbus.node[" << getIndex() << "] erg�nzen" << endl;
                    }
                }
            }
            if(mode != 2){
                EV << "Nachricht " << id << " eingetragen" << endl;
                char index[64];
                int wantedidx;
                int idx = 0;
                for (vector<MOB*>::iterator it=out_m.begin(); it != out_m.end(); ++it){
                    MOB *idtemp = *it;
                    if(idtemp->getID() == id){
                        wantedidx = idx;
                    }
                    idx++;
                }
                sprintf(index, "%d", wantedidx);
                MOB *mob = out_m.back();
                double nextsend = mob->getPeriod()/(double)1000;
                if(nextsend != 0){
                    //EV << "Nachricht " << id << " wird zu Zeit " << nextsend << " versandt" << endl;
                    cMessage *self = new cMessage(index);
                    scheduleAt(nextsend, self);
                }else{
                    //Nachricht nur auf Abruf
                }
            }
        }else{
            EV << "Nicht genug Parameter f�r die Nachrichten von Node " << node << " angegeben." << endl;
            if(!idtkn.hasMoreTokens()){
                EV << "In omnetpp.ini unter Canbus.node[" << getIndex() << "].id erg�nzen." << endl;
            }
            if(!mdtkn.hasMoreTokens()){
                EV << "In omnetpp.ini unter Canbus.node[" << getIndex() << "].mode erg�nzen." << endl;
            }
            if(!prtkn.hasMoreTokens()){
                EV << "In omnetpp.ini unter Canbus.node[" << getIndex() << "].periodicity erg�nzen." << endl;
            }
            if(!dltkn.hasMoreTokens()){
                EV << "In omnetpp.ini unter Canbus.node[" << getIndex() << "].datalength erg�nzen." << endl;
            }
            if(!dttkn.hasMoreTokens()){
                EV << "In omnetpp.ini unter Canbus.node[" << getIndex() << "].data erg�nzen." << endl;
            }
            endSimulation();
        }
    }
    EV << endl;
}

string canNodeApp::idbinary(int id){
        char resid[30] = {0};
        int index = 29;

        memset((void*)resid, '0', 29);
        do{
            resid[--index] = '0' + (id & 1);
        }while(id >>= 1);

        return string(&resid[0]);
}
