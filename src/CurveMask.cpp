#include "CurveMask.h"

int CurveMask::getMask(CurveType type) {
	int i = static_cast<int>(type);
	return 1 << i;
}

int CurveMask::combineMask(std::list<CurveType> types) {
	int mask = 0;
	for (CurveType type : types) {
		mask |= getMask(type);
	}
	return mask;
}

int CurveMask::getAllBits() {
	return 15;
}

bool CurveMask::isInMask(CurveType type, int mask) {
	return mask & getMask(type);
}