package fico4omnet.synchronisation.flexray;

//
// Dummy synchronisation module that corrects the clock with a certain imprecission.
// It uses the simulation time as basis.
//
// @see IPort
//
// @author Till Steinbach
simple FRSync
{
    parameters:
        @display("i=block/timer");
        
        int pOffsetCorrectionOut = default(15);
		int pRateCorrectionOut = default(3);
		int pClusterDriftDamping = default(0);
}
