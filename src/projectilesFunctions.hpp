#pragma once
#include <math.h>
#include <iostream>

#define PI 3.14159265359
namespace projectilesFunctions
{
	double projectileAnim(double value)
	{
		return std::pow(sin(value * PI / 180.), 2);
	}
}

