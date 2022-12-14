#include "stdafx.h"
#include "BasicDefinitions.hpp"

const std::array<const char*, 14> GRADE_MARK_STR = { "D", "C", "B", "A", "A+", "AA", "AA+", "AAA", "AAA+", "S", "995", "998", "999", "PUC" };
const std::array<const char*, 6> CLEAR_MARK_STR = { "None", "Played", "Clear", "EXClear", "UC", "PUC" };

static_assert(static_cast<int>(GradeMark::PUC) + 1 == sizeof(GRADE_MARK_STR) / sizeof(GRADE_MARK_STR[0]),
	"GradeMark and GRADE_MARK_STR must contain same # of elements.");

static_assert(static_cast<int>(ClearMark::Perfect) + 1 == sizeof(CLEAR_MARK_STR) / sizeof(CLEAR_MARK_STR[0]),
	"ClearMark and CLEAR_MARK_STR must contain same # of elements.");