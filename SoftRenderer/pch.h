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
#include <memory>

#define SAFE_DELETE(p) if(p) delete p; p=nullptr;