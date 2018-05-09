#pragma once

#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include <g3log/loglevels.hpp>
#include "util/console_util.h"
#include <iostream>
namespace g3
{
    static const int kErrorValue = 800;
}  // g3

namespace terra
{
    const LEVELS G3LOG_ERROR{g3::kErrorValue, {"ERROR"}};

    struct ConsoleSink 
	{
		EConsoleColor GetConsoleColor(const LEVELS level) const
		{
			if (level.value == G3LOG_DEBUG.value) {
				return EConsoleColor::DEFAULT;
			}
			if (level.value == INFO.value) {
				return EConsoleColor::GREEN;
			}
			if (level.value == WARNING.value) {
				return EConsoleColor::YELLOW;
			}
			if (level.value == G3LOG_ERROR.value) {
				return EConsoleColor::RED;
			}
			if (g3::internal::wasFatal(level)) {
				return EConsoleColor::PINK;
			}
			return EConsoleColor::WHITE;
		}
        void ReceiveLogMessage(g3::LogMessageMover logEntry) { 
			auto level = logEntry.get()._level;
			EConsoleColor color = GetConsoleColor(level);
			ConsoleUtils::SetColor(color);
			std::cout << logEntry.get().toString();
			ConsoleUtils::SetColor(EConsoleColor::DEFAULT);
		}
    };

#define G3LOG_DEFAULT_INIT(log_prefix, log_directory)                     \
    \
std::unique_ptr<g3::LogWorker>                                            \
        g3_log_worker = g3::LogWorker::createLogWorker();                 \
    \
auto handle = g3_log_worker->addDefaultLogger(log_prefix, log_directory); \
    \
g3::initializeLogging(g3_log_worker.get());                               \
    g3_log_worker->addSink(std::make_unique<ConsoleSink>(), &ConsoleSink::ReceiveLogMessage)
}