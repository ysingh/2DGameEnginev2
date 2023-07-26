#define _CRT_SECURE_NO_WARNINGS
#include "Logger.h"

// TODO(yudi) : At some point I should make a ansi color format specifier.
// Terminal color format specifier
// "\033[{FORMAT_ATTRIBUTE};{FORGROUND_COLOR};{BACKGROUND_COLOR}m{TEXT}\033[{RESET_FORMATE_ATTRIBUTE}m"

std::vector<LogEntry> Logger::messages;

void Logger::Log(const std::string& msg) {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	LogEntry logEntry;
	logEntry.type = LOG_INFO;
	logEntry.msg_timestamp_timePoint = now;
	logEntry.msg = "LOG | " + Logger::GetCurrentTimeStampString(now) + msg;

	std::cout << "\033[0;32;49m";
	std::cout << logEntry.msg << std::endl;
	std::cout << "\033[0m";

	messages.push_back(logEntry);
}

void Logger::Err(const std::string& msg) {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	LogEntry logEntry;
	logEntry.type = LOG_ERROR;
	logEntry.msg_timestamp_timePoint = now;
	logEntry.msg = "ERR | " + Logger::GetCurrentTimeStampString(now) + msg;

	std::cout << "\033[0;31;49m";
	std::cerr << logEntry.msg << std::endl;
	std::cout << "\033[0m";

	messages.push_back(logEntry);
}

const std::string Logger::GetCurrentTimeStampString(const std::chrono::system_clock::time_point now) {
	std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
	std::string now_str(32, '\0');
	strftime(&now_str[0], 32, "%d-%b-%Y %H:%M:%S - ", localtime(&timestamp));
	return now_str;
}
