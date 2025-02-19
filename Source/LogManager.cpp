#include "LogManager.h"
#include "Constants.h"

std::mutex LogManager::OutputMutex;

LogManager* LogManager::GetInstance()
{
	static LogManager Instance;
	return &Instance;
}

void LogManager::LogDebugTrace(const std::string& InString)
{
#if DEBUG_MODE
	std::lock_guard<std::mutex> lock(OutputMutex);
	std::cout << InString << std::endl;
#endif
}

void LogManager::LogDebugTrace(const std::string& InString, const std::string& InFunctionName)
{
#if DEBUG_MODE
	LogDebugTrace(InFunctionName + ": " + InString);
#endif
}

void LogManager::LogProfileTrace(const std::string& InName, double InDurationInMicrosec, double InDutarionInMillisec)
{
#if DEBUG_MODE
	std::string Name = InName;
	while (Name.length() < 40) Name.append(" ");

	std::string DurationAsStr = std::to_string(static_cast<float>(InDurationInMicrosec / 1000.f));
	DurationAsStr = DurationAsStr.substr(0, DurationAsStr.length() - (DurationAsStr.length() - 6));

	std::string HashLine;
	for (int i = 0; i < static_cast<int>(InDutarionInMillisec); ++i) HashLine.append("#");

	LogManager::LogDebugTrace(std::string("PMC - " + Name + " | " + DurationAsStr + "ms " + HashLine));
#endif
}
