#pragma once

#include <baseapp/crash_tracer.h>
#include <memory>

namespace plog
{
    class IAppender;
}

namespace baseapp
{

class Application
{
public:
    Application(const char* modulePath, bool traceToFile = true, bool verbose = true);

private:
    CrashTracer m_crashTracer;
    std::unique_ptr<plog::IAppender> m_logAppender;
};

}