//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __FICO4OMNET_FRSYNC_H_
#define __FICO4OMNET_FRSYNC_H_

#include <omnetpp.h>
#include <list>

namespace FiCo4OMNeT {

/**
 * @brief sync module.
 *
 */
class FRSync : public cSimpleModule
{
	private:
		enum T_Channel{
			A,B
		};
		enum T_EvenOdd{
			EVEN, ODD
		};
		struct T_DevValid{
			int value;
			bool valid;
		};
		struct Positions{
			int line;
			int frameID;
		};

		int zOffsetCorrection;
		int zRateCorrection;
		int pOffsetCorrectionOut;
		int pRateCorrectionOut;
		int pClusterDriftDamping;
		static const int MAXSYNCNODES = 15;
		std::list<int> position;

		T_DevValid ***T_DevTable;

    protected:
        /**
         * @brief Initializes the dummy synchronization module.
         *
         * registers an event in the scheduler to trigger the synchronization
         *
         */
        virtual void initialize();

        /**
         * @brief Returns the clock correction value.
         */
        virtual int ftmAlgorithm(std::list<int> zList);

        /**
		 * @brief Returns the line number in witch the Deviation Value has to be stored.
		 */
        virtual unsigned int getLineNr(int frameID);


    public:
        /**
		 * @brief Returns the offset correction value zOffsetCorrection
		 */
		virtual int offsetCorrectionCalculation(unsigned int vCycleCounter);

		/**
		 * @brief Returns the rate correction value zRateCorrection
		 */
		virtual int rateCorrectionCalculation();

        /**
         * @brief Stores the deviation values to the sync nodes in T_DevTable.
         */
        virtual void storeDeviationValue(int frameID, int zEO, int zCh, int value, bool valid);

        /**
         * @brief If the node is a sync node the value zero is saved in T_DevTable.
         */
        virtual void storeOwnSyncFrame(int frameID, int zEO);

        /**
         * @brief Deletes the list position and sets all valid variables in T_DevTable to false.
         */
        virtual void resetTables();
};

}
#endif
