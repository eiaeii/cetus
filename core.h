#pragma once

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>
typedef int socklen_t;
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#define ioctlsocket ioctl
typedef int SOCKET;
constexpr int SOCKET_ERROR = -1;
constexpr int INVALID_SOCKET = -1;
#endif



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
#include <mutex>
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
#include "global_variables.h"

typedef std::make_signed<size_t>::type ssize_t;

