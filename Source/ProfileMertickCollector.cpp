#include "ProfileMertickCollector.h"

ProfileMerticsCollector::ProfileMerticsCollector(const std::string& InName)
{
	Name = InName;
	Start = std::chrono::high_resolution_clock::now();
}

ProfileMerticsCollector::~ProfileMerticsCollector()
{
	auto End = std::chrono::high_resolution_clock::now();
	auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>(End - Start);
	std::string DurationAsStr = std::to_string(Duration.count());
	DebugTrace(std::string("PMC - Name: " + Name + "\t\tDuration: " + DurationAsStr + "ms"));
}
