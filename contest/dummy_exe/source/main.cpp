#include <boost/optional.hpp>

#include <dummy/dummy.h>
#include <tclap/CmdLine.h>

#include <memory>

#include <cmath>

#include <SFML/Graphics.hpp>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

#include <iostream>
#include <thread>

#include <boost/multiprecision/cpp_int.hpp>
#include <lemon/list_graph.h>

#include <logger/logger.hpp>
int main(int argc, char *argv[])
{
    std::unique_ptr<logger::ILogger> logger {logger::createMainLogger()};
    logger->Error("test");
    TCLAP::CmdLine cmdLine {"dummy program to test externals", '=', "2016.8"};

    TCLAP::SwitchArg testSwitch {"t","test","test switch", false};
    cmdLine.add(testSwitch);

    cmdLine.parse(argc, argv);

    std::cout << "Flag value " << testSwitch.getValue() << std::endl;

    Dummy d;

   auto f = stlab::async(stlab::default_executor, [] { return 42; });
    // Waiting just for illustrational purpose
    while (!f.get_try()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "The answer is " << f.get_try().value() << "\n";

    using namespace boost::multiprecision;

    cpp_int bi = 1;
    // Do some arithmetic:
    for(unsigned i = 1; i <= 1000; ++i)
        bi *= i;

    std::cout << bi << std::endl; // prints 1000!
    using namespace lemon;
    ListDigraph g;
    ListDigraph::Node u = g.addNode();
    ListDigraph::Node v = g.addNode();
    ListDigraph::Arc  a = g.addArc(u, v);
    std::cout << "Hello World! This is LEMON library here." << std::endl;
    std::cout << "We have a directed graph with " << countNodes(g) << " nodes "
              << "and " << countArcs(g) << " arc." << std::endl;

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
