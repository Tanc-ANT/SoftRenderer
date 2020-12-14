#pragma once
#include <basetsd.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>
#include <cassert>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>
#include <algorithm>

inline int CLAMP(int x, int min, int max) { return (x < min) ? min : ((x > max) ? max : x); }
inline float CLAMP(float x, float min, float max) { return (x < min) ? min : ((x > max) ? max : x); }

#define SAFE_DELETE(p) if(p) delete p; p=nullptr;