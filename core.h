#pragma once

#include <iostream>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <cstdarg>
#include <functional>
#include <type_traits>
#include <memory>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <thread>
#include <atomic>
#include <sstream>

#include <algorithm>
#include <cmath>
#include <limits>
#include "global_macro.h"

#include "guid/fguid.h"

#include "gsl_assert.h"
#include "type_traits_ex.h"
#include "math/math_ex.h"

typedef std::make_signed<size_t>::type ssize_t;
