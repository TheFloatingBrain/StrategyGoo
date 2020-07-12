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
	Goo& goo = manager.CreateEntity< Goo >( BoardPosition( 8, 8 ) );
	Goo::GooComponent& splot = goo.AddGoo( BoardPosition( 8, 9 ) );
	goo.AddGoo( BoardPosition( 8, 10 ) );
	sf::Clock frameRateController;
	float frameRate = 1.f / 60.f;
	sf::View camera = window.getDefaultView();
//	camera.move( )
	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) ) {
			if( event.type == sf::Event::Closed )
				window.close();
		}
		manager.Update( window );
		if( frameRateController.getElapsedTime().asSeconds() >= frameRate ) {
			frameRateController.restart();
			manager.Render( window );
		}
	}
	return 0;
}


