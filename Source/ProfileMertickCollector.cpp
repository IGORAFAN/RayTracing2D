#include "ProfileMertickCollector.h"
#include "Helper.h"
#include "LogManager.h"

ProfileMerticsCollector::ProfileMerticsCollector(const std::string& InName)
{
	Name = InName;
	Start = std::chrono::high_resolution_clock::now();
}

ProfileMerticsCollector::~ProfileMerticsCollector()
{
	auto End = std::chrono::high_resolution_clock::now();

	auto DurationInMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(End - Start);
	auto DurationInMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(End - Start);

	double DurationInMicrosecAsDouble = DurationInMicroseconds.count();
	double DutarionInMillisecAsDouble = DurationInMilliseconds.count();

	LogManager::LogProfileTrace(Name, DurationInMicrosecAsDouble, DutarionInMillisecAsDouble);
}
