#pragma once

#include <memory>
#include <stdint.h>

#define SNOW_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)