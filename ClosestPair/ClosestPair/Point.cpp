#include "Point.h"



Point::Point()
{
}


Point::~Point()
{
}

void Point::operator=(const Point & P)
{
	x = P.x;
	y = P.y;
}

bool Point::operator==(const Point & P)
{
	return (x == P.x && y == P.y);
}

Point::Point(float a, float b)
{
	x = a;
	y = b;
}

