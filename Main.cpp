#include <SFML/Graphics.hpp>
#include <iostream>
#include "Sprite.hpp"

void PrintRect( sf::IntRect rect )
{
	std::cout << rect.left << ", " << rect.top << 
			", " << rect.width << ", " << rect.height << "\n";
}

int main( int argc, char** args )
{
	sf::RenderWindow window( sf::VideoMode( 1024, 768 ), "sf::RenderWindow" );

	PrintRect( StrategyGoo::Sprite::LoadSpriteImage( "Squaddie" ) );
	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) ) {
			if( event.type == sf::Event::Closed )
				window.close();
		}
		window.draw( sf::Sprite( *StrategyGoo::Detail::masterTexture ) );
		window.clear();
		window.display();
	}

	return 0;
}
