#include "Interval.h"

const Interval Interval::s_Empty = Interval(std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
const Interval Interval::s_Universe = Interval(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());