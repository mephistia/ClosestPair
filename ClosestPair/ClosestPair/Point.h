#pragma once
class Point
{
public:
	Point();
	~Point();

	void operator = (const Point &P);

	bool operator == (const Point &P);


	Point(float a, float b);

	float x, y;

};

