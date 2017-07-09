#include <dll_template/dummy_dll.h>
#include <dll_template/log.h>
#include <tclap/CmdLine.h>

#include <plog/Appenders/ConsoleAppender.h>

#include <SFGUI/Button.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Widgets.hpp>

int main(int argc, char *argv[])
{
    TCLAP::CmdLine cmdLine {"dummy program to test externals", '=', "2016.8"};

    cmdLine.parse(argc, argv);

    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    dll_template_log::initializePlog(plog::debug, plog::get());

    DummyDll dll;
    
    sf::RenderWindow app_window(sf::VideoMode(800, 600), "App window");
    app_window.resetGLStates();
    
    sfg::SFGUI sfgui;
    sfg::Desktop desktop;
    
  
    
    auto sfml_scrollable_window = sfg::Window::Create( );
	sfml_scrollable_window->SetTitle( "SFML scrollable canvas" );
	sfml_scrollable_window->SetPosition( sf::Vector2f( 0.f, 0.f ) );
    
    auto sfml_scrollable_canvas = sfg::Canvas::Create();
    auto horizontal_scrollbar = sfg::Scrollbar::Create( sfg::Scrollbar::Orientation::HORIZONTAL );
	auto vertical_scrollbar = sfg::Scrollbar::Create( sfg::Scrollbar::Orientation::VERTICAL );
    
    auto table = sfg::Table::Create();
	table->Attach( sfml_scrollable_canvas, sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), 
                   sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL |sfg::Table::EXPAND );
	table->Attach( vertical_scrollbar, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), 0, sfg::Table::FILL );
	table->Attach( horizontal_scrollbar, sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, 0 );
    
      // Create a box with 20 pixels spacing.
    auto alignment = sfg::Alignment::Create();
	auto alignment_box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );
	alignment_box->Pack( alignment, true, true );
    alignment->Add( table );
    
    sfml_scrollable_window->Add( alignment );

    
    const static auto scrollable_canvas_size = 300.f;
    sf::View view( sf::Vector2f( 0.f, 0.f ), sf::Vector2f( scrollable_canvas_size, scrollable_canvas_size ) );
    
    auto horizontal_adjustment = horizontal_scrollbar->GetAdjustment();
	auto vertical_adjustment = vertical_scrollbar->GetAdjustment();
	horizontal_adjustment->SetLower( scrollable_canvas_size / 2.f );
	horizontal_adjustment->SetUpper( 218.f * 20 - scrollable_canvas_size / 2.f );
	horizontal_adjustment->SetMinorStep( 20.f );
	horizontal_adjustment->SetMajorStep( scrollable_canvas_size );
	horizontal_adjustment->SetPageSize( scrollable_canvas_size );
	vertical_adjustment->SetLower( scrollable_canvas_size / 2.f );
	vertical_adjustment->SetUpper( 84.f * 20 - scrollable_canvas_size / 2.f );
	vertical_adjustment->SetMinorStep( 20.f );
	vertical_adjustment->SetMajorStep( scrollable_canvas_size );
	vertical_adjustment->SetPageSize( scrollable_canvas_size );

	horizontal_adjustment->GetSignal( sfg::Adjustment::OnChange ).Connect( [&view, &horizontal_adjustment]() {
		view.setCenter( horizontal_adjustment->GetValue(), view.getCenter().y );
	} );

	vertical_adjustment->GetSignal( sfg::Adjustment::OnChange ).Connect( [&view, &vertical_adjustment]() {
		view.setCenter( view.getCenter().x, vertical_adjustment->GetValue() );
	} );
    
    sfml_scrollable_canvas->SetRequisition( sf::Vector2f( scrollable_canvas_size, scrollable_canvas_size ) );
    
    desktop.Add( sfml_scrollable_window );


    sf::Clock clock;
    desktop.Update(0.f);

    app_window.setFramerateLimit(25);
    while (app_window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (app_window.pollEvent(event))
        {
            
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                app_window.close();
            }
            else
            {
                desktop.HandleEvent(event);
            }
        }
        
        
        LOG_INFO << "Render cycle";
        
        // Update the GUI every 5ms
		if( clock.getElapsedTime().asMicroseconds() >= 5000 ) {
			// Update() takes the elapsed time in seconds.
			desktop.Update( static_cast<float>( clock.getElapsedTime().asMicroseconds() ) / 1000000.f );

			clock.restart();
		}
       
        app_window.clear(sf::Color::Black);
        
        sfml_scrollable_canvas->Bind();
		sfml_scrollable_canvas->Clear( sf::Color( 0, 0, 0, 0 ) );
		sfml_scrollable_canvas->SetView( view );
        
        sfml_scrollable_canvas->Draw(sf::RectangleShape(sf::Vector2f(120, 50)));
        
        app_window.setActive(true);
        
        app_window.draw(sf::RectangleShape(sf::Vector2f(290,150)));

        
        sfgui.Display( app_window );
        app_window.display();
    }

    return 0;
}
