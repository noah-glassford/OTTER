#pragma once
#include <GLFW/glfw3.h>
class Timer
{
public:
	static void Tick();
	static float dt;
	static float PreviousTime;
};