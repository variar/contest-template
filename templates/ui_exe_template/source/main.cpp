#include <dll_template/dummy_dll.h>
#include <dll_template/log.h>
#include <tclap/CmdLine.h>

#include <plog/Appenders/ConsoleAppender.h>

int main(int argc, char *argv[])
{
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    dll_template_log::initializePlog(plog::debug, plog::get());
    
    TCLAP::CmdLine cmdLine {"dummy program to test externals", '=', "2016.8"};

    TCLAP::SwitchArg testSwitch {"t","test","test switch", false};
    cmdLine.add(testSwitch);

    cmdLine.parse(argc, argv);

    DummyDll dll;

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(25);
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
        
        LOG_INFO << "Render cycle";
        window.clear(sf::Color::Black);
        window.display();
    }

    return 0;
}
