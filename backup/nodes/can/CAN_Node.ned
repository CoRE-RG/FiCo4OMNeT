package can_fr.nodes.can;

// Represents a participating node of the CAN-Network
// 
// Generates data-frames and tries to send them via the CAN_Bus to other nodes
simple CAN_Node
{
    parameters:
        bool extcode = default(false);			// Activate if external code shall be used (has to be deactivated in this version)
        bool payload = default(false);			// Activate if payload shall be enabled
        string version = default("2.0A");		// Version of CAN-Protocol. 2.0A = 11 Bits identifier, 2.0B = 29 Bits identifier
        int percentage = default(50);			// For Bit-Stuffing mode 2
        int bitstuffing = default(0);			// Bit-Stuffing mode. See .ini files for further information
        bool errors = default(false);			// Activate if errors shall be enabled (must be activated for CAN_Bus and CAN_Node)
        int errorperc = default(10);			// If errors are enabled: percentage of errors
        bool ack = default(false);				// Activate if ack-recognition of receiving nodes shall be enabled (must be activated for CAN_Bus and CAN_Node)
        int bandwidth = default(1000000);		// valid values are 10000 - 1000000 (must be same for CAN_Bus and CAN_Node)
        int numOfMessages = default(1);			// Number of messages of the node
        int numOfMobs = default(2);				// Number of physical message buffers
		bool trafficGen = default(true);		// Activate if traffic-generator shall be used (must be activated in this version)	
        string data;			// The data - String parameter (string) separated with spaces
        string id;				// The ID(s) - String parameter (int) separated with commas
        string periodicity; 	// period(s) for sending of messages - String parameter (int) separated with commas
        string mode;			// mode - String parameter (int) separated with commas
        string datalength;		// Datafield-length - String parameter (int) separated with commas
        string node;			// Name of the node
        @display("i=block/app2");
    gates:
        inout gate;
}