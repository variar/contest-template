#include <baseapp/application.h>

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>

#include <absl/strings/string_view.h>
#include <absl/strings/str_split.h>
#include <absl/strings/str_format.h>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#define _getpid getpid
#endif

namespace baseapp
{

namespace
{

std::string GetLogName(const char* modulePath)
{
    const std::vector<absl::string_view> pathParts = absl::StrSplit(modulePath, absl::ByAnyChar("/\\"));

	time_t now = time(nullptr);
	tm* t = localtime(&now);
    return absl::StrFormat("%s_%02d-%02d-%02d_%d.log",
        pathParts.empty() ? "log" : pathParts.back(),
        t->tm_hour, t->tm_min, t->tm_sec, _getpid()
    );
}

}

Application::Application(const char* modulePath, bool traceToFile, bool verbose)
    : m_crashTracer(modulePath)
{
#ifndef BASEAPP_DISABLE_TRACE
    if (traceToFile)
    {
        m_logAppender = std::make_unique<plog::RollingFileAppender<plog::TxtFormatter>>(GetLogName(modulePath).c_str(), 10000000, 1);
    }
    else
    {
        m_logAppender = std::make_unique<plog::ConsoleAppender<plog::TxtFormatter>>(plog::OutputStream::kCerr);
    }

    plog::init(verbose ? plog::debug : plog::info, m_logAppender.get());
#endif
}

}