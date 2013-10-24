#include "CanID.h"

	CanID::CanID(int id, cModule *node, simtime_t signInTime, bool rtr, bool remotesent): id(id), node(node), signInTime(signInTime), rtr(rtr), remotesent(remotesent){}
		
	int CanID::getId(){
		return id;
	}
		
	cModule* CanID::getNode(){
		return node;
	}
	
	bool CanID::getRtr(){
		return rtr;
	}
	
	bool CanID::getRemotesent(){
		return remotesent;
	}
	
	simtime_t CanID::getSignInTime(){
		return signInTime;
	}
	
