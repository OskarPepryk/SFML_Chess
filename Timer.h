#pragma once
#include <chrono>
#include <iostream>

class Timer
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
public:
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		std::cout << "Timer took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms.\n";
	}
};