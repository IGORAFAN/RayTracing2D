#pragma once
#include <chrono>
#include <string>
#include "Helper.h"

class ProfileMerticsCollector
{
public:
	ProfileMerticsCollector() = delete;
	ProfileMerticsCollector(const std::string& InName);
	~ProfileMerticsCollector();

protected:
	std::string Name;
	std::chrono::time_point<std::chrono::high_resolution_clock> Start;
};

#if DEBUG_MODE
#define PROFILE_METRICS_COLLECTOR(Name) ProfileMerticsCollector PMC(Name);
#else
#define PROFILE_METRICS_COLLECTOR(Name)
#endif