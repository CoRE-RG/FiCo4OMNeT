
// Sent by a receiving node to confirm the frame
//
message AckMsg{
	bool ack; 	// true if correct
	int id;		// id of the message
}
