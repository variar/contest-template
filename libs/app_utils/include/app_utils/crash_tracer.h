#pragma once

namespace app_utils
{

struct CrashTracer
{
    CrashTracer(const char* modulePath);

    CrashTracer(const CrashTracer&) = delete;
    CrashTracer(CrashTracer&&) = delete;

    CrashTracer& operator=(const CrashTracer&) = delete;
    CrashTracer& operator=(CrashTracer&&) = delete;
};

}