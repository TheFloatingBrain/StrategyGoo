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
	GameplayManager* manager = new GameplayManager{ registry };
	sf::Clock frameRateController;
	float frameRate = 1.f / 60.f;
	sf::View camera = window.getDefaultView();
	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) )
		{
			if( event.type == sf::Event::Closed )
				window.close();
			if( event.type = sf::Event::KeyReleased )
			{
				if( manager->GetGameState() == GameplayManager::StagesOfPlay::WIN ||
					manager->GetGameState() == GameplayManager::StagesOfPlay::LOOSE ) {
					registry = entt::registry();
					manager = new GameplayManager{ registry };
				}
			}
		}
		if( frameRateController.getElapsedTime().asSeconds() >= frameRate )
		{
			manager->Update( window );
			frameRateController.restart();
			manager->Render( window );
		}
	}
	return 0;
}


