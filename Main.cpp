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

int main( int argc, char** args )
{
	sf::RenderWindow window( sf::VideoMode( 1024, 768 ), "sf::RenderWindow" );
	entt::registry registry;
	GameplayManager manager{ registry };
	manager.CreateEntity< Squaddie >( BoardPosition( 1, 1 ) ).RefrenceSprite().SetCurrentDirection( Direction::SOUTH );
	manager.CreateEntity< Squaddie >( BoardPosition( 4, 1 ) ).RefrenceSprite().SetCurrentDirection( Direction::NORTH );
	manager.CreateEntity< Squaddie >( BoardPosition( 1, 3 ) ).RefrenceSprite().SetCurrentDirection( Direction::WEST );
	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) ) {
			if( event.type == sf::Event::Closed )
				window.close();
		}
		manager.Render( window );
		size_t count = 0;
		registry.view< Squaddie::SquaddieRefrence, Tile::TileRefrence >().each( [&]( Squaddie::SquaddieRefrence&, Tile::TileRefrence& ) { ++count; } );
		std::cout << "Tiles with squaddies: " << count << "\n";
	}

	return 0;
}


