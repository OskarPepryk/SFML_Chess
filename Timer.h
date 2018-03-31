#pragma once
#include <chrono>
#include <iostream>
#include <string>

class Timer
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	std::string name;
public:
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	Timer(std::string name) : name{ name }
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		std::cout << "Timer " << name << " took " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us.\n";
	}
};