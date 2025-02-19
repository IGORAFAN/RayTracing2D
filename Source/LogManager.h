#pragma once

#include <string>
#include <iostream>
#include "Helper.h"
#include <mutex>

class LogManager
{
public:
	static LogManager* GetInstance();

	static void LogDebugTrace(const std::string& InString);
	static void LogDebugTrace(const std::string& InString, const std::string& InFunctionName);
	static void LogProfileTrace(const std::string& InName, double DurationInMicrosecAsInt, double DutarionInMillisecAsInt);

private:
	LogManager() = default;
	LogManager(const LogManager&) = delete;
	LogManager(const LogManager&&) = delete;
	LogManager& operator=(const LogManager&) = delete;
	LogManager& operator=(const LogManager&&) = delete;

	static std::mutex OutputMutex;
};

