#include <baseapp/crash_tracer.h>

#include <absl/debugging/failure_signal_handler.h>
#include <absl/debugging/symbolize.h>

namespace baseapp
{

CrashTracer::CrashTracer(const char* modulePath)
{
    absl::InitializeSymbolizer(modulePath);
    absl::FailureSignalHandlerOptions options;
    absl::InstallFailureSignalHandler(options);
}

}