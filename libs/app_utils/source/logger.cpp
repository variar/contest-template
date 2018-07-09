#include <app_utils/logger.h>

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>

#include <absl/strings/string_view.h>
#include <absl/strings/str_split.h>
#include <absl/strings/str_format.h>
#include <absl/strings/str_cat.h>

#include <absl/time/clock.h>

#include <app_utils/crash_tracer.h>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#define _getpid getpid
#endif

namespace app_utils
{

namespace
{

plog::util::nstring GetLogName(const char* modulePath)
{
    const std::vector<absl::string_view> pathParts = absl::StrSplit(modulePath, absl::ByAnyChar("/\\"));

    auto filename = absl::StrFormat(
        "%s_%s_%d.log",
        pathParts.empty() ? "log" : pathParts.back(),
        absl::FormatTime("%m.%d_%H.%M", absl::Now(), absl::LocalTimeZone()),
        _getpid()
    );

#ifdef _WIN32
        return plog::util::toWide(filename.c_str());
#else
        return filename;
#endif

}

using namespace plog;
class Formatter
{
public:
    static util::nstring header()
    {
        return m_header;
    }

    static util::nstring format(const Record& record)
    {
        util::nostringstream ss;

        const auto traceString =  absl::StrCat(
                absl::FormatTime("%H:%M:%E3S", absl::Now(), absl::LocalTimeZone()), "\t",
                severityToString(record.getSeverity()), "\t",
                absl::StrFormat("[%05.d]", record.getTid()), "\t"
                "[", record.getFunc(), "@", record.getLine(), "] "
        );

#ifdef _WIN32
        ss << plog::util::toWide(traceString.c_str());
#else
        ss << traceString;
#endif

        ss << record.getMessage() << "\n";
        return ss.str();
    }

    static void PrepareHeader(int argc, char** argv)
    {
        std::string header = "Command line: ";
        for (auto i = 0; i<argc; ++i)
        {
            absl::StrAppend(&header, argv[i]);
        }

#ifdef _WIN32
        absl::StrAppend(&header, "\r\n\r\n");     
        m_header = plog::util::toWide(header.c_str());  
#else
        absl::StrAppend(&header, "\n\n");
        m_header = std::move(header);
#endif 
    }

private:
    static util::nstring m_header;
};

util::nstring Formatter::m_header = {};

}

Logger::Logger(int argc, char** argv, LoggerChannel channel, LoggerFlags flags)
{
    if (IsSet(flags, LoggerFlags::CrashTrace))
    {
        m_crashTracer = std::make_unique<CrashTracer>(argv[0]);
    }

    const auto logLevel = IsSet(flags, LoggerFlags::Verbose) ? plog::debug : plog::info;

    switch(channel)
    {
        case LoggerChannel::File:
            m_logAppender = std::make_unique<plog::RollingFileAppender<Formatter>>(GetLogName(argv[0]).c_str(), 10000000, 1);
            break;
        case LoggerChannel::Console:
            m_logAppender = std::make_unique<plog::ConsoleAppender<Formatter>>(plog::OutputStream::kCerr);
            break;
        default:
            break;
    }

    if (m_logAppender)
    {
        Formatter::PrepareHeader(argc, argv);
        plog::init(logLevel, m_logAppender.get());
    }
}

Logger::~Logger() = default;

}