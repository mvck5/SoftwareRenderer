#include "timer.h"

Timer::Timer() :
	m_start{ std::chrono::high_resolution_clock::now() },
	m_end{ std::chrono::high_resolution_clock::now() },
	m_duration{ m_start - m_end } {
}

void Timer::start() {
	m_start = std::chrono::high_resolution_clock::now();
}

void Timer::end() {
	m_end = std::chrono::high_resolution_clock::now();
	m_duration = m_end - m_start;
}

double Timer::getDuration() {
	return m_duration.count();
}