#pragma once
#include<chrono>
#include<iostream>
#include<Windows.h>
class Timer
{
public:
	void start()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
		m_bRunning = true;
	}

	void stop()
	{
		m_EndTime = std::chrono::high_resolution_clock::now();
		m_bRunning = false;
	}

	double elapsedMilliseconds()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

		if (m_bRunning)
		{
			endTime = std::chrono::high_resolution_clock::now();
		}
		else
		{
			endTime = m_EndTime;
		}

		return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
	}

	double elapsedMicroseconds()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> endTime;

		if (m_bRunning)
		{
			endTime = std::chrono::high_resolution_clock::now();
		}
		else
		{
			endTime = m_EndTime;
		}

		return std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_StartTime).count();
	}

	double elapsedSeconds()
	{
		return elapsedMilliseconds() / 1000.0;
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_EndTime;
	bool m_bRunning = false;
};


bool detectKeyStateNoWait(char key)
{
	bool pressed = false;
	if (GetKeyState(key) & 0x8000)
		pressed = true;

	return pressed;
}


bool detectKeyStateWaitUnpress(char key)
{
	bool pressed = false;
	if (GetKeyState(key) & 0x8000)
	{
		pressed = true;
		while (GetKeyState(key) & 0x8000);
	}

	return pressed;
}
