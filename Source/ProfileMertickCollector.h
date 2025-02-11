#pragma once
#include <chrono>
#include <string>
#include "Constants.h"

class ProfileMerticsCollector
{
public:
	ProfileMerticsCollector(const std::string& InName);
	~ProfileMerticsCollector();

private:
	ProfileMerticsCollector() = delete;
	ProfileMerticsCollector(const ProfileMerticsCollector&) = delete;
	ProfileMerticsCollector(const ProfileMerticsCollector&&) = delete;
	ProfileMerticsCollector& operator=(const ProfileMerticsCollector&) = delete;
	ProfileMerticsCollector& operator=(const ProfileMerticsCollector&&) = delete;

protected:
	std::string Name;
	std::chrono::time_point<std::chrono::high_resolution_clock> Start;
};

#if DEBUG_MODE
#define PROFILE_METRICS_COLLECTOR(Name) ProfileMerticsCollector PMC(Name);
#else
#define PROFILE_METRICS_COLLECTOR(Name)
#endif