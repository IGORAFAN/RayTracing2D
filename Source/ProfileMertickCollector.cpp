#include "ProfileMertickCollector.h"

ProfileMerticsCollector::ProfileMerticsCollector(const std::string& InName)
{
	Name = InName;
	Start = std::chrono::high_resolution_clock::now();
}

ProfileMerticsCollector::~ProfileMerticsCollector()
{
	auto End = std::chrono::high_resolution_clock::now();

	while (Name.length() < 40) Name.append(" ");

	auto DurationInMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(End - Start);
	std::string DurationAsStrInMilliseconds = std::to_string(static_cast<float>(DurationInMicroseconds.count() / 1000.f));
	DurationAsStrInMilliseconds = DurationAsStrInMilliseconds.substr(0, DurationAsStrInMilliseconds.length() - (DurationAsStrInMilliseconds.length() - 6));
	//while (DurationAsStrInMilliseconds.length() < 6) DurationAsStrInMilliseconds.append("0");

	std::string HashLine;
	auto DurationInMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(End - Start);
	int DutarionAsInt = DurationInMilliseconds.count();
	for (int i = 0; i < static_cast<int>(DutarionAsInt); ++i) HashLine.append("#");

	DebugTrace(std::string("PMC - Name: " + Name + " | " + DurationAsStrInMilliseconds + "ms " + HashLine));
}
