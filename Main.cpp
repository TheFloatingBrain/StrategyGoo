#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Sprite.hpp"

void PrintRect( sf::IntRect rect )
{
	std::cout << rect.left << ", " << rect.top << 
			", " << rect.width << ", " << rect.height << "\n";
}

int main( int argc, char** args )
{
	sf::RenderWindow window( sf::VideoMode( 1024, 768 ), "sf::RenderWindow" );

	StrategyGoo::Sprite test{ "Squaddie" };

	( *test ).setPosition( 100, 100 );
	int directions = 0;
	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) ) {
			if( event.type == sf::Event::Closed )
				window.close();
		}
		test.Draw( window );
		sf::sleep( sf::Time( sf::seconds( 1 ) ) );
		test.SetCurrentDirection( ( StrategyGoo::Direction ) 
			( directions++ % StrategyGoo::AMOUNT_OF_DIRECTIONS_CONSTANT ) );
		window.display();
		window.clear();
	}

	return 0;
}
