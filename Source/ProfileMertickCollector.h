#pragma once
#include <chrono>
#include <string>
#include "Helper.h"

class ProfileMerticsCollector
{
public:
	ProfileMerticsCollector(const std::string& InName);
	~ProfileMerticsCollector();

protected:
	std::string Name;
	std::chrono::time_point<std::chrono::high_resolution_clock> Start;
};

#define PROFILE_METRICS_COLLECTOR ProfileMerticsCollector PMC(__FUNCTION__);
