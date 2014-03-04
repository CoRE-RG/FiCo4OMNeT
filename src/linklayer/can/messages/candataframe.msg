
// Represents a data-frame
//
// contains information about the sending node, data etc.
packet CanDataFrame{
	string node;		// related node
	int canID;				// ID of the message
    int length;			// length of the message in bit
	bool rtr;			// true if remote-frame
	char data[8];		// if payload is activated, this contains the data
	simtime_t startTime;// for stats collection purposes
	int period;			// Periodicy of the message 
}