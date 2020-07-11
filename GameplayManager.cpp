#include "GameplayManager.hpp"

namespace StrategyGoo
{
	GameplayManager::GameplayManager() : gameBoard( registry, 64, 64 ) {}
	void GameplayManager::Update()
	{
		Render();
	}
	void GameplayManager::Render()
	{
		registry.view< Tile::TileRefrence, Sprite >().each( [&]( auto& tile, auto& sprite ) {
			sprite.Draw( window );
			}
		);
	}
}