#include <crash_handler/crash_tracer.h>

#define BOOST_STACKTRACE_LINK

#include <signal.h>

#include <boost/stacktrace.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

namespace {
    void SignalHandler(int signum) {
        ::signal(signum, SIG_DFL);
        boost::stacktrace::safe_dump_to("./backtrace.dump");
        ::raise(SIGABRT);
    }
}

    CrashTracer::CrashTracer()
    {
        if (boost::filesystem::exists("./backtrace.dump")) {
            // there is a backtrace
            std::ifstream ifs("./backtrace.dump");

            boost::stacktrace::stacktrace st = boost::stacktrace::stacktrace::from_dump(ifs);
            std::cout << "Previous run crashed:\n" << st << std::endl;

            // cleaning up
            ifs.close();
            boost::filesystem::remove("./backtrace.dump");
        }

        ::signal(SIGSEGV, &SignalHandler);
        ::signal(SIGABRT, &SignalHandler);
    }
