#include <Timer.h>

float Timer::dt;
float Timer::PreviousTime;

void Timer::Tick()
{
	float time = static_cast<float>(glfwGetTime());
	dt = time - PreviousTime;
	PreviousTime = time;
}