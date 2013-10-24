#include "Buffer.h"

Buffer::Buffer(int id, int mode): id(id), mode(mode) {}

int Buffer::getId(){
	return id;
}

int Buffer::getMode(){
	return mode;
}

void Buffer::setData(char datain[]){
	for(int i = 0; i<8; i++){
		data[i] = datain[i];
	}
}

char* Buffer::getData(){
	return data;
}
