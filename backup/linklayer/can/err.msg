
// Used for error signaling
// 
// Sent by nodes and forwarded by the bus to signal errors to all participating nodes
message ErrorFrame{
	int id;		// ID of the message
	int node;	// related node with error
	int kind; //Sender: 0: Bit-Error, 1: Form-Error; Receiver: 2: CRC-Error, 3: Bit-Stuffing-Error
	int pos;	// position of the error in the corresponding data-frame
}
