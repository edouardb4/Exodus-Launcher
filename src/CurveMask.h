#pragma once

#include <list>
#include "CurveType.h"

namespace CurveMask {
	int getMask(CurveType type);

	int combineMask(std::list<CurveType> types);

	int getAllBits();

	bool isInMask(CurveType type, int mask);
};