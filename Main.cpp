#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GameplayManager.hpp"

using namespace StrategyGoo;

void PrintRect( sf::IntRect rect )
{
	std::cout << rect.left << ", " << rect.top << 
			", " << rect.width << ", " << rect.height << "\n";
}

void Render( entt::registry& registry, sf::RenderWindow& window );

int main( int argc, char** args )
{
	sf::RenderWindow window( sf::VideoMode( 1024, 768 ), "sf::RenderWindow" );

	entt::registry registry;

	GameBoard board{ registry, 64, 64 };

	Squaddie squad( registry, BoardPosition{ 10, 10 }, &board, board.GetTileWidthConstant(), board.GetTileHeightConstant() );

	std::cout << Detail::masterTexture->getSize().x << ", " << Detail::masterTexture->getSize().y << "\n";

	//sf::Clock clock;

	//sf::Time time = clock.getElapsedTime();

	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) ) {
			if( event.type == sf::Event::Closed )
				window.close();
		}
		//const auto temp = clock.getElapsedTime();
		//std::cout << ( temp - time ).asMilliseconds() << "\n";

		//window.draw( sf::Sprite{ *Detail::masterTexture } );
		//window.display();
		//window.clear();
		Render( registry, window );
	}

	return 0;
}

void Render( entt::registry& registry, sf::RenderWindow& window )
{
	registry.sort< Sprite >( []( Sprite& first, Sprite& second ) {
		return first.GetLayer() < second.GetLayer();
	} );
	window.clear();
	registry.view< Sprite >().each( [&]( Sprite& sprite ) {
		sprite.Draw( window );
		}
	);
	window.display();
}

