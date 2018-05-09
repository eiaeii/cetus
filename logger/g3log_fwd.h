#pragma once

#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>

namespace terra
{
	struct ConsoleSink
	{
		void ReceiveLogMessage(g3::LogMessageMover logEntry) {
			std::cout << logEntry.get().toString();
		}
	};

#define G3LOG_DEFAULT_INIT(log_prefix, log_directory)              \
    \
std::unique_ptr<g3::LogWorker>                                     \
        g3_log_worker = g3::LogWorker::createLogWorker();          \
    \
auto handle = g3_log_worker->addDefaultLogger(log_prefix, log_directory); \
    \
g3::initializeLogging(g3_log_worker.get()); \
	g3_log_worker->addSink(std::make_unique<ConsoleSink>(), &ConsoleSink::ReceiveLogMessage)
}