#pragma once

#include <string>
#include <algorithm>
#include <cassert>

#include "oxygine-framework.h"
using namespace oxygine;

// IDs
typedef unsigned int NumericId;

// Copy prevention
class Uncopyable
{
protected:
	Uncopyable() {}
	~Uncopyable() {}

private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};

// Math
template <typename T> int sign(T value)
{
	return (T(0) < value) - (value < T(0));
}