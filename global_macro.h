#pragma once

namespace terra
{
    static constexpr int INDEX_NONE = -1;

#define MAKE_INSTANCE(classname)    \
    \
public:                             \
    static classname* GetInstance() \
    {                               \
        static classname obj;       \
        return &obj;                \
    }

#define DISABLE_COPY(classname)                      \
    \
public:                                              \
    classname(const classname&) = delete;            \
    classname& operator=(const classname&) = delete; \
    classname(classname&&) = default;

#define G3LOG_DEFAULT_INIT(log_prefix, log_directory)              \
    \
std::unique_ptr<g3::LogWorker>                                     \
        g3_log_worker = g3::LogWorker::createLogWorker();          \
    \
auto handle = g3_log_worker->addDefaultLogger(log_prefix, log_directory); \
    \
g3::initializeLogging(g3_log_worker.get())

}