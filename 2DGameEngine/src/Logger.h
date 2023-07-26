/*
   * Examples of a professional logger
   * C++: https://github.com/gabime/spdlog\
   * C: https://github.com/rxi/log.c
*/

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <vector>

enum LogType {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

struct LogEntry {
	LogType type;
	std::string msg;
	std::chrono::system_clock::time_point msg_timestamp_timePoint;
};

class Logger {
private:
	static std::vector<LogEntry> messages;
	static const std::string GetCurrentTimeStampString(const std::chrono::system_clock::time_point now);
public:
	static void Log(const std::string& msg);
	static void Err(const std::string& msg);
};
#endif