#pragma once

// gotta love intellisense :)
// this can be removed when compiling it just 
// makes intellisense aware that we are
// compiling with C++17 
#define _HAS_CXX17 true
#define _HAS_CXX20 false

#include <Window.h>

#include <algorithm>
#include <any>
#include <array>
#include <array>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <shared_mutex>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <entt/entity/entity.hpp>

#include "util/stb_image.h"
