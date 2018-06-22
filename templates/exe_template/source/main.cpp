#include <thread>
#include <cli11/cli11.hpp>

#include <lib_template/dummy.h>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

#include <lemon/list_graph.h>

#include <plog/Appenders/ConsoleAppender.h>

#include <gsl/gsl_util.h>

#include <mpir.h>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include <crash_handler/crash_tracer.h>

int main(int argc, char *argv[])
{
   CrashTracer{argv[0]};

    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    CLI::App app{"dummy program to test externals"};

    app.add_flag_function("-c,--crash",
             [](size_t) {int* a = nullptr; *a = 0;},
             "crash switch");
   
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    Dummy d;

   auto f_mpir = stlab::async(stlab::default_executor, []
    { 
        using namespace boost::multiprecision;
        mpq_rational v = 1;

        LOG_INFO << "MPIR start";
        // Do some arithmetic:
        for(unsigned i = 1; i <= 1000; ++i)
            v *= i;

        LOG_INFO << "MPIR done" << v;
    });

    auto f_boost = f_mpir.then(stlab::default_executor,[] {
        using namespace boost::multiprecision;
        cpp_rational v = 1;

        LOG_INFO << "BOOST start";
        // Do some arithmetic:
        for (unsigned i = 1; i <= 1000; ++i)
            v *= i;

        LOG_INFO << "BOOST done"; // prints 1000!
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
