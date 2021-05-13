#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include <utility>
#include <functional>
#include <filesystem>
#include <chrono>
#include <fstream>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <cstdint>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>

#include "Sanguine/Core/Log.h"
#include "Sanguine/Core/Instrumentation.h"

#ifdef SG_PLATFORM_WINDOWS
	#include <Windows.h>
#endif