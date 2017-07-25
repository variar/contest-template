#include <thread>
#include <tclap/CmdLine.h>

#include <lib_template/dummy.h>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

#include <lemon/list_graph.h>

#include <plog/Appenders/ConsoleAppender.h>

#include <gsl/gsl_util.h>

#include <mpir.h>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <boost/stacktrace.hpp>

int main(int argc, char *argv[])
{
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    TCLAP::CmdLine cmdLine {"dummy program to test externals", '=', "2016.8"};

    TCLAP::SwitchArg testSwitch {"t","test","test switch", false};
    cmdLine.add(testSwitch);

    cmdLine.parse(argc, argv);

    Dummy d;

   auto f_mpir = stlab::async(stlab::default_executor, []
    { 
        using namespace boost::multiprecision;
        mpq_rational v = 1;

        LOG_INFO << "MPIR start";
        // Do some arithmetic:
        for(unsigned i = 1; i <= 10000; ++i)
            v *= i;

        LOG_INFO << "MPIR done";
    });

    auto f_boost = f_mpir.then(stlab::default_executor,[] {
        using namespace boost::multiprecision;
        cpp_rational v = 1;

        LOG_INFO << "BOOST start";
        // Do some arithmetic:
        for (unsigned i = 1; i <= 10000; ++i)
            v *= i;

        LOG_INFO << "BOOST done"; // prints 1000!

        LOG_INFO << boost::stacktrace::stacktrace();

        return 42;
    });

    // Waiting just for illustrational purpose
    while (!f_boost.get_try()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "The answer is " << f_boost.get_try().value() << "\n";

    {
        auto guard = gsl::finally([]() { LOG_INFO << "guard done"; });
    }

    using namespace lemon;
    ListDigraph g;
    ListDigraph::Node u = g.addNode();
    ListDigraph::Node v = g.addNode();
    g.addArc(u, v);
    LOG_INFO <<  "Hello World! This is LEMON library here.";

    return 0;
}
