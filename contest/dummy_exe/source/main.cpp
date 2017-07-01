#include <boost/optional.hpp>

#include <dummy/dummy.h>
#include <tclap/CmdLine.h>

#include <iostream>

#include <cmath>

#include <SFML/Graphics.hpp>

#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

#include <iostream>
#include <thread>

struct mytask {
  mytask(size_t n)
    :_n(n)
  {}
  void operator()() {
    for (int i=0;i<10000;++i) {}  // Deliberately run slow
    std::cerr << "[" << _n << "]";
  }
  size_t _n;
};

int main(int argc, char *argv[])
{
  	TCLAP::CmdLine cmdLine {"dummy program to test externals", '=', "2016.8"};
	
	TCLAP::SwitchArg testSwitch {"t","test","test switch", false};
	cmdLine.add(testSwitch);
	
	cmdLine.parse(argc, argv);
	
	std::cout << "Flag value " << testSwitch.getValue() << std::endl;
	
	Dummy d;

    tbb::task_scheduler_init init(tbb::task_scheduler_init::default_num_threads());  // Explicit number of threads

    std::vector<mytask> tasks;
    for (int i=0;i<10;++i)
        tasks.push_back(mytask(i));

        tbb::parallel_for(
            tbb::blocked_range<size_t>(0,tasks.size()),
            [&tasks](const tbb::blocked_range<size_t>& r) {
                for (size_t i=r.begin();i<r.end();++i) tasks[i]();
            }
        );

     auto f = stlab::async(stlab::default_executor, [] { return 42; });
      // Waiting just for illustrational purpose
    while (!f.get_try()) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }

    std::cout << "The answer is " << f.get_try().value() << "\n";
    
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
