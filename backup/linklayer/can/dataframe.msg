
// Represents a data-frame
//
// contains information about the sending node, data etc.
message DataFrame{
	string node;		// related node
	int gateId;			// internal information for message forwarding
	int identifier;		// ID of the message
    int length;			// length of the message
	bool rtr;			// true if remote-frame
	char data[8];		// if payload is activated, this contains the data
	simtime_t startTime;// for stats collection purposes
}