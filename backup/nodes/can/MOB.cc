#include "MOB.h"

	//Keine original stuffing bits, kein Payload
	MOB::MOB(int mode, int id, int periodicity, int datalength, int bs, int percentage, bool version)
	: mode(mode), id(id), periodicity(periodicity), datalength(datalength), version(version) {
			//EV << "Konstruktor 1" << endl;
			payload = false;
			calc_length(bs, percentage);
			if(mode == 2){	//Zu empfangende Nachricht
				periodicity = 0;
			}
	}
	
	//Original Stuffing bits, kein Payload:
	MOB::MOB(int mode, int id, int periodicity, int datalength, int bs, int percentage, bool version, string idbin)
	: mode(mode), id(id), periodicity(periodicity), datalength(datalength), version(version) {
			//EV << "Konstruktor 2" << endl;
			payload = false;
			createbinary(idbin);
			calc_length(bs, percentage);
	}
	
	//Keine original stuffing bits, Payload aktiviert:
	MOB::MOB(int mode, int id, int periodicity, char datain[], int bs, int percentage, bool version)
	: mode(mode), id(id), periodicity(periodicity), version(version) {
			//EV << "Konstruktor 3" << endl;
			payload = true;
			for(int i = 0; i<8; i++){
				data[i] = datain[i];
			}
			int size = 0;
			for(int i = 0; i<8; i++){
				if(data[i] == 0){
					break;
				}else{
					size++;
				}
			}
			datalength = size;
			calc_length(bs, percentage);
	}
	
	//Original Stuffing bits mit payload:
	MOB::MOB(int mode, int id, int periodicity, char datain[], int bs, int percentage, bool version, string idbin)
	: mode(mode), id(id), periodicity(periodicity), version(version) {
			//EV << "Konstruktor 4" << endl;
			payload = true;
			for(int i = 0; i<8; i++){
				data[i] = datain[i];
			}
			int size = 0;
			for(int i = 0; i<8; i++){
				if(data[i] == 0){
					break;
				}else{
					size++;
				}
			}
			datalength = size;
			createbinary(idbin);
			calc_length(bs, percentage);
	}
	
	void MOB::calc_length(int bs, int percentage){
		// Berechnung zur Länge
		int idlength;
		if(version){
			idlength = 0;	//11Bit Identifier
		}else{
			idlength = 20;	//29Bit Identifier - 20 Bits mehr
		}
		switch(bs){
			//Für keine stuffing bits:
			case 0: length = 47 + idlength + (datalength<<3); //47 Bits ist die Laenge des Frames ohne Daten. Daten * 8 für bits
			break;
			//stuffing bits worst case:
			case 1: length = 47 + idlength + (datalength<<3) + (29+ idlength + (datalength<<3))/4 + 1;	//Der hintere Teil steht fuer die Stopfbits
			break;
			//Fuer eine prozentuale Verteilung:
			case 2: length = 47 + idlength + (datalength<<3) + (((29+ idlength + (datalength<<3))/4 + 1) * ((double)percentage/100));
			break;
			case 3: length = 47 + idlength + (datalength<<3) + binstuffbits();
			break;
			case 4: length = 0;
			break;
			default: length = 0;
			break;
		} 
	}
	
	int MOB::binstuffbits(){
		int result = 0;
		int counter = 0;
		char cmp = '0';
		int binsize = binary.size();
		for(int i = 1; i<binsize; i++){
			if(binary.at(i) == cmp){
				counter++;
			}else{
				counter = 0;
			}
			if(counter == 4){	//5 gleiche Bits hintereinander
				result++;
				counter = 0;
				if(cmp == '0'){
					cmp = '1';
				}else{
					cmp = '0';
				}
			}else{				//noch keine 5 gleichen Bits
				cmp = binary.at(i);
			}
		}
		/*EV << result << " Stopfbits für die ID " << id << endl;
		EV << endl;*/
		return result;
	}
	
	void MOB::createbinary(string idbin){
		
//############## 11 Bit Identifier: ############################
		if(version){
			//SOF
			binary.push_back('0');
			//ID
			for(int i = 18; i<29; i++){
				binary.push_back(idbin[i]);
			}
			//RTR:
			if(mode == 0){
				binary.push_back('0'); 	//Data-Frame
			}else{						// mode == 1
				binary.push_back('1'); 	//Remote-Frame
			}
			//Reserved Bits
			for(int i = 0; i<2; i++){
				binary.push_back('1');
			}
			//Datalength Control Field
			char resid[5] = {0};
			int index = 4;
			int templength = datalength;
			memset((void*)resid, '0', 4);
			do{
				resid[--index] = '0' + (templength & 1);
			}while(templength >>= 1);
			for(int i = 0; i<4; i++){
				binary.push_back(resid[i]);
			}
			//Daten
			char rand = '0';
			if(payload){
				for(int i = 0; i<datalength; i++){
					char resdata[9] = {0};
					int idx = 8;
					char tempdata = data[i];
					memset((void*)resdata, '0', 8);
					do{
						resdata[--idx] = '0' + (tempdata & 1);
					}while(tempdata >>= 1);
					for(int j = 0; j<8; j++){
						binary.push_back(resdata[j]);
					}
				}
			}else{
				templength = datalength*8;	//Länge in Bits
				for(int i = 0; i<templength; i++){
					binary.push_back(rand);
					if(rand == '0'){
						rand = '1';
					}else rand = '0';
				}
			}
			//CRC - abwechselnd einsen und nullen
			
			for (int i = 0; i<15; i++){
				binary.push_back(rand);
				if(rand == '0'){
					rand = '1';
				}else rand = '0';
			}
			
			/*EV << "Der Daten-Frame " << id << " binär: " << endl;
			int binsize = binary.size();
			for(int i = 0; i < binsize; i++){
				EV << " " << binary.at(i);
			}
			EV << endl;*/
//#############################################################
//-------------------------------------------------------------		
//############## 29-Bit-Identifier: ###########################
		}else{			
			//SOF
			binary.push_back('0');
			//ID - teil 1
			for(int i = 0; i<11; i++){
				binary.push_back(idbin[i]);
			}
			//Reserved Bits
			for(int i = 0; i<2; i++){
				binary.push_back('1');
			}
			//ID - teil 2
			for(int i = 11; i<29; i++){
				binary.push_back(idbin[i]);
			}
			//RTR:
			if(mode == 0){
				binary.push_back('0'); 	//Data-Frame
			}else{						// mode == 1
				binary.push_back('1'); 	//Remote-Frame
			}
			//Reserved Bits
			for(int i = 0; i<2; i++){
				binary.push_back('0');
			}
			//Datalength Control Field
			char resid[5] = {0};
			int index = 4;
			int templength = datalength;
			memset((void*)resid, '0', 4);
			do{
				resid[--index] = '0' + (templength & 1);
			}while(templength >>= 1);
			for(int i = 0; i<4; i++){
				binary.push_back(resid[i]);
			}
			//Daten
			char rand = '0';
			if(payload){
				for(int i = 0; i<datalength; i++){
					char resdata[9] = {0};
					int idx = 8;
					char tempdata = data[i];
					memset((void*)resdata, '0', 8);
					do{
						resdata[--idx] = '0' + (tempdata & 1);
					}while(tempdata >>= 1);
					for(int j = 0; j<8; j++){
						binary.push_back(resdata[j]);
					}
				}
			}else{		//einsen und nullen abwechselnd
				templength = datalength*8;	//Länge in Bits
				for(int i = 0; i<templength; i++){
					binary.push_back(rand);
					if(rand == '0'){
						rand = '1';
					}else rand = '0';
				}
			}
			//CRC
			for (int i = 0; i<15; i++){
				binary.push_back(rand);
				if(rand == '0'){
					rand = '1';
				}else rand = '0';
			}
			
			/*EV << "Der Daten-Frame " << id << " binär: " << endl;
			int binsize = binary.size();
			for(int i = 0; i < binsize; i++){
				EV << " " << binary.at(i);
			}
			EV << endl;*/
		}
//##############################################################
	}
	
	int MOB::getID(){
		return id;
	}
	
	int MOB::getMode(){
		return mode;
	}
	
	char* MOB::getData(){
		return data;
	}
	
	int MOB::getPeriod(){
		return periodicity;
	}
	
	int MOB::getLength(){
		return length;
	}
	
