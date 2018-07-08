#include <app_utils/crash_tracer.h>

#include <absl/debugging/failure_signal_handler.h>
#include <absl/debugging/symbolize.h>

namespace app_utils
{

CrashTracer::CrashTracer(const char* modulePath)
{
#ifndef BASEAPP_DISABLE_TRACE

    absl::InitializeSymbolizer(modulePath);
    absl::FailureSignalHandlerOptions options;
    absl::InstallFailureSignalHandler(options);

#endif
}

}