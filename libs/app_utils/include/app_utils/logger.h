#pragma once

#include <memory>

namespace plog
{
    class IAppender;
}

namespace app_utils
{

struct CrashTracer;

enum class LoggerFlags
{
    None = 0x0,
    CrashTrace = 0x1,
    Verbose = 0x2
};

inline LoggerFlags operator |(LoggerFlags lhs, LoggerFlags rhs)  
{
    return static_cast<LoggerFlags> (
        static_cast<std::underlying_type<LoggerFlags>::type>(lhs) |
        static_cast<std::underlying_type<LoggerFlags>::type>(rhs)
    );
}

inline bool IsSet(LoggerFlags flags, LoggerFlags flag)  
{
    return !!(
        static_cast<std::underlying_type<LoggerFlags>::type>(flags) &
        static_cast<std::underlying_type<LoggerFlags>::type>(flag)
    );
}

enum class LoggerChannel
{
    None,
    File,
    Console,
};

class Logger
{
public:
    Logger(int argc, char** argv, LoggerChannel channel, LoggerFlags flags);
    ~Logger();

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

private:
    std::unique_ptr<CrashTracer> m_crashTracer;
    std::unique_ptr<plog::IAppender> m_logAppender;
};

}