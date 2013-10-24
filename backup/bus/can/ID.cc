#include "ID.h"

	ID::ID(int id, int node, simtime_t signInTime, bool rtr, bool remotesent): id(id), node(node), signInTime(signInTime), rtr(rtr), remotesent(remotesent){}
		
	int ID::getId(){
		return id;
	}
		
	int ID::getNode(){
		return node;
	}
	
	bool ID::getRtr(){
		return rtr;
	}
	
	bool ID::getRemotesent(){
		return remotesent;
	}
	
	simtime_t ID::getSignInTime(){
		return signInTime;
	}
	