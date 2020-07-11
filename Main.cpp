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
	manager.CreateSquaddie( BoardPosition( 1, 1 ) ).sprite->SetCurrentDirection( Direction::SOUTH );
	manager.CreateSquaddie( BoardPosition( 4, 1 ) ).sprite->SetCurrentDirection( Direction::NORTH );
	manager.CreateSquaddie( BoardPosition( 1, 3 ) ).sprite->SetCurrentDirection( Direction::WEST );
	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) ) {
			if( event.type == sf::Event::Closed )
				window.close();
		}
		manager.Render( window );
	}

	return 0;
}


