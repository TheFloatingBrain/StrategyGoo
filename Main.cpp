#include <SFML/Graphics.hpp>

int main( int argc, char** args )
{
	sf::RenderWindow window( sf::VideoMode( 960, 540 ), "sf::RenderWindow" );
	sf::RectangleShape rectangle( sf::Vector2f( 100.0f, 200.0f ) );
	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) ) {
			if( event.type == sf::Event::Closed )
				window.close();
		}
		window.clear();
		window.draw( rectangle );
		window.display();
	}

	return 0;
}
