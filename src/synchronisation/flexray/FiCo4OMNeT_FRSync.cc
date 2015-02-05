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

#include "FiCo4OMNeT_FRSync.h"
#include "FiCo4OMNeT_FRScheduler.h"
#include "FiCo4OMNeT_SchedulerMessageEvents_m.h"

namespace FiCo4OMNeT {

Define_Module( FRSync);


void FRSync::initialize() {
    pOffsetCorrectionOut = par("pOffsetCorrectionOut");
    pRateCorrectionOut = par("pRateCorrectionOut");
    pClusterDriftDamping = par("pClusterDriftDamping");
    zOffsetCorrection = 0;
    zRateCorrection = 0;
    // Allocate memory
    T_DevTable = new T_DevValid**[2];
    for (int i = 0; i < 2; ++i) {
        T_DevTable[i] = new T_DevValid*[2];

        for (int j = 0; j < 2; ++j)
            T_DevTable[i][j] = new T_DevValid[MAXSYNCNODES];
    }
    resetTables();
}

int FRSync::offsetCorrectionCalculation(unsigned int vCycleCounter) {
	std::list<int> zsMListAB;
	T_EvenOdd zEO;
	if (vCycleCounter % 2 == 0) {
		zEO = EVEN;
	} else {
		zEO = ODD;
	}

	for (int i = 0; i < MAXSYNCNODES; i++) {
		if (T_DevTable[zEO][A][i].valid) {
			if (T_DevTable[zEO][B][i].valid) {
				zsMListAB.push_back(
						std::min(T_DevTable[zEO][A][i].value,
								T_DevTable[zEO][B][i].value));
			} else {
				zsMListAB.push_back(T_DevTable[zEO][A][i].value);
			}
		} else if (T_DevTable[zEO][B][i].valid) {
			zsMListAB.push_back(T_DevTable[zEO][B][i].value);
		}
	}
	if (zsMListAB.size() == 0) {
		zOffsetCorrection = 0;
	} else {
		zOffsetCorrection = ftmAlgorithm(zsMListAB);
	}

	if (zOffsetCorrection > pOffsetCorrectionOut) {
		zOffsetCorrection = pOffsetCorrectionOut;
	} else if (zOffsetCorrection < -pOffsetCorrectionOut) {
		zOffsetCorrection = -pOffsetCorrectionOut;
	}
	return zOffsetCorrection;
}

int FRSync::rateCorrectionCalculation() {
	std::list<int> zsMRateAB;
	for (int i = 0; i < MAXSYNCNODES; i++) {
		if (T_DevTable[EVEN][A][i].valid && T_DevTable[ODD][A][i].valid) {
			if (T_DevTable[EVEN][B][i].valid && T_DevTable[ODD][B][i].valid) {
				zsMRateAB.push_back(
						(T_DevTable[ODD][A][i].value
								- T_DevTable[EVEN][A][i].value
								+ T_DevTable[ODD][B][i].value
								- T_DevTable[EVEN][B][i].value) / 2);
			} else {
				zsMRateAB.push_back(
						T_DevTable[ODD][A][i].value
								- T_DevTable[EVEN][A][i].value);
			}
		} else if (T_DevTable[EVEN][B][i].valid
				&& T_DevTable[ODD][B][i].valid) {
			zsMRateAB.push_back(
					T_DevTable[ODD][B][i].value - T_DevTable[EVEN][B][i].value);
		}
	}

	if (zsMRateAB.size() > 0) {
		zRateCorrection += ftmAlgorithm(zsMRateAB);
        if (zRateCorrection >= pClusterDriftDamping) {
            zRateCorrection -= pClusterDriftDamping;
        } else if (zRateCorrection <= -pClusterDriftDamping) {
            zRateCorrection += pClusterDriftDamping;
        } else {
            zRateCorrection = 0;
        }
	}

	if (zRateCorrection > pRateCorrectionOut) {
		zRateCorrection = pRateCorrectionOut;
	} else if (zRateCorrection < -pRateCorrectionOut) {
		zRateCorrection = -pRateCorrectionOut;
	}
	return zRateCorrection;
}

int FRSync::ftmAlgorithm(std::list<int> zList) {
	size_t zLength = zList.size();
	std::list<int>::iterator it1, it2;
	if (zLength == 0) {
		return 0;
	} else {
		zList.sort();
		it1 = zList.begin();
		it2 = zList.end();
		it2--;
		if (zLength > 7) {
			it1++;
			it2--;
			it1 = zList.erase(zList.begin(), ++it1);
			it2 = zList.erase(it2, zList.end());
		} else if (zLength >= 3) {
			it1 = zList.erase(it1);
			it2 = zList.erase(it2);
		}
		return (zList.front() + zList.back()) / 2;
	}
}

size_t FRSync::getLineNr(int frameID) {
	std::list<int>::iterator it1 = position.begin();
	size_t zPos = 0;
	for (size_t i = 0; i < position.size(); i++) {
		if (*it1 == frameID) {
			zPos = i;
			i = position.size();
		} else {

			it1++;
			zPos = i + 1;
		}
	}
	if (zPos >= position.size()) {
		position.push_back(frameID);
	}
	if (zPos > MAXSYNCNODES-1) {
		opp_error("too many sync nodes");
	}
	return zPos;
}

void FRSync::storeDeviationValue(int frameID, int zEO, int zCh, int value,
		bool valid) {
	Enter_Method_Silent();
	size_t zPos = getLineNr(frameID);
	if (T_DevTable[zEO][zCh][zPos].valid) {
		opp_error("multiple sync nodes in slot %d",frameID);
	} else {
		T_DevTable[zEO][zCh][zPos].value = value;
		T_DevTable[zEO][zCh][zPos].valid = valid;
	}
}

void FRSync::storeOwnSyncFrame(int frameID, int zEO){
	Enter_Method_Silent();
	size_t zPos = getLineNr(frameID);
	if (T_DevTable[zEO][0][zPos].valid && T_DevTable[zEO][1][zPos].valid) {
		opp_error("multiple sync nodes in slot %d",frameID);
	} else {
			T_DevTable[zEO][0][zPos].value = 0;
			T_DevTable[zEO][0][zPos].valid = true;
			T_DevTable[zEO][1][zPos].value = 0;
			T_DevTable[zEO][1][zPos].valid = true;
	}
}

void FRSync::resetTables() {
	for (int i = 0; i < MAXSYNCNODES; ++i) {
		T_DevTable[EVEN][A][i].valid = false;
		T_DevTable[EVEN][B][i].valid = false;
		T_DevTable[ODD][A][i].valid = false;
		T_DevTable[ODD][B][i].valid = false;
	}
	position.clear();
}

}
