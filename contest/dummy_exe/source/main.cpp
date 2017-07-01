#include <boost/optional.hpp>

#include <dummy/dummy.h>
#include <tclap/CmdLine.h>

#include <iostream>

#include <cmath>

#include <SFML/Graphics.hpp>

int main(int argc, char *argv[])
{
  	TCLAP::CmdLine cmdLine {"dummy program to test externals", '=', "2016.8"};
	
	TCLAP::SwitchArg testSwitch {"t","test","test switch", false};
	cmdLine.add(testSwitch);
	
	cmdLine.parse(argc, argv);
	
	std::cout << "Flag value " << testSwitch.getValue() << std::endl;
	
	Dummy d;

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
