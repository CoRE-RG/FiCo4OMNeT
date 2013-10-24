#include "Stats.h"

Stats::Stats(int id): id(id){
	numProcessed = 0;
	min = 0;
	max = 0;
	currMean = 0;
	init = false;
}

void Stats::newTime(simtime_t time){
	if(!init){
		min = max = currMean = time;
		init = true;
		numProcessed++;
		totalTime = time;
	}else{
		if(time > max) max = time;
		if(time < min) min = time;
		totalTime += time;
		numProcessed++;
		currMean = totalTime/numProcessed;
	}
}

simtime_t Stats::getMax(){
	return max;
}

simtime_t Stats::getMin(){
	return min;
}

simtime_t Stats::getMean(){
	return currMean;
}

int Stats::getId(){
	return id;
}
