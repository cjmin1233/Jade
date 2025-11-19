#pragma once
//  
#include <iostream>
#include <sstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

// Data Structure
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

// Jade api
#include "Jade/Core/Base.h"
#include "Jade/Core/Log.h"
#include "Jade/Debug/Instrumentor.h"

#ifdef JADE_PLATFORM_WINDOWS
#include <Windows.h>
#endif // JADE_PLATFORM_WINDOWS