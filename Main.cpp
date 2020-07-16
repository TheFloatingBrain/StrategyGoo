#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GameplayManager.hpp"

using namespace BioGooContainmentSquad;

void PrintRect( sf::IntRect rect )
{
	std::cout << rect.left << ", " << rect.top << 
			", " << rect.width << ", " << rect.height << "\n";
}

int main( int argc, char** args )
{
	sf::RenderWindow window( sf::VideoMode( 1024, 768 ), "Bio Goo Containment Squad" );
	entt::registry registry;
	GameplayManager* manager = new GameplayManager{ registry };
	sf::Clock frameRateController;
	float frameRate = 1.f / 60.0f;
	sf::View camera = window.getDefaultView();
	while( window.isOpen() )
	{
		sf::Event event;
		while( window.pollEvent( event ) )
		{
			if( event.type == sf::Event::Closed )
				window.close();
			if( event.type == sf::Event::KeyPressed )
			{
				if( manager->GetGameState() == GameplayManager::StagesOfPlay::WIN ||
					manager->GetGameState() == GameplayManager::StagesOfPlay::LOOSE )
				{
					registry = entt::registry();
					delete manager;
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
	delete manager;
	BioGooContainmentSquad::CleanUpTextureMemory();
	return 0;
}


