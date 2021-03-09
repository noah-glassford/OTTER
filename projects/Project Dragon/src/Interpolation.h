#pragma once //Any type of interpolation goes into here
static class Interpolation
{
public:
	static float LERP(float v0, float v1, float t)
	{
		return v0 + t * (v1 - v0);
	}
};
