#include <thread>
#include <tclap/CmdLine.h>

#include <lib_template/dummy.h>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

#include <lemon/list_graph.h>

#include <plog/Appenders/ConsoleAppender.h>

#include <gsl/gsl_util.h>

int main(int argc, char *argv[])
{
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    TCLAP::CmdLine cmdLine {"dummy program to test externals", '=', "2016.8"};

    TCLAP::SwitchArg testSwitch {"t","test","test switch", false};
    cmdLine.add(testSwitch);

    cmdLine.parse(argc, argv);

    Dummy d;

   auto f = stlab::async(stlab::default_executor, [] 
    { 
        LOG_INFO << "Logging from async";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        LOG_INFO << "Async done";

        return 42; 
    });
   
    // Waiting just for illustrational purpose
    while (!f.get_try()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "The answer is " << f.get_try().value() << "\n";

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
