
// Used by CAN_Node and CAN_Bus to communicate arbitration processes.
//
// Contains information about message-id and related nodes
message ArbMsg{
	int id;		// ID of the message
	int node;	// related node for the arbitration process
	bool rtr;	// true if remote-frame
	bool remotesent; //if the data-frame will be sent after a remote request
	simtime_t signInTime;	// time of the sending wish of the node
}
