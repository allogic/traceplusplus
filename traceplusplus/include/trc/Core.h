#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <typeinfo>
#include <memory>
#include <utility>
#include <concepts>
#include <queue>
#include <bitset>

typedef bool                   b8;

typedef char                   s8;
typedef unsigned char          u8;

typedef int                    s32;
typedef unsigned int           u32;
typedef long long int          s64;
typedef unsigned long long int u64;

typedef std::bitset<128>       u128;

typedef std::uintptr_t         p64;

typedef float                  r32;
typedef double                 r64;

typedef glm::fvec2             r32v2;
typedef glm::fvec3             r32v3;
typedef glm::fvec4             r32v4;

typedef glm::fmat4             r32m4;