#include "Timer.h"
#include <thread>

Timer::Timer()
{
	m_Start = std::chrono::high_resolution_clock::now();
}

const float Timer::GetDuration() const
{
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> duration = end - m_Start;

	float ms = duration.count();

	return ms;
}
