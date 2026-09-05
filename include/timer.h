#pragma once
#include <chrono>

class Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	std::chrono::time_point<std::chrono::steady_clock> m_end;
	std::chrono::duration<double> m_duration;
public:
	Timer();

	void start();
	void end();
	double getDuration();
};

