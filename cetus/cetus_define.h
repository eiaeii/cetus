#pragma once

#include <iostream>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <string.h>

#include <stdarg.h>

#include <cmath>
#include <functional>

#if ASSERT_AS_EXCEPTION
#define CETUS_ASSERT(Expr, Msg) if(!(Expr)) throw std::runtime_error(Msg);
#else
#define CETUS_ASSERT(Expr, Msg) assert(Expr && Msg)
#endif