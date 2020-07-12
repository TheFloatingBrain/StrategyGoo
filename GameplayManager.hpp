#ifndef GAMEPLAY_MANAGER_HEADER_HPP
#define GAMEPLAY_MANAGER_HEADER_HPP
#include "Goo.hpp"

namespace StrategyGoo
{
	struct GameplayManager
	{
		GameplayManager( entt::registry& registry_, size_t width = 64, size_t height = 64 );
		void Update();
		void Render( sf::RenderWindow& window );
		template< typename ENTITY_TYPE >
		ENTITY_TYPE& CreateEntity( BoardPosition startingPosition );
		entt::registry& RefrenceRegistry();
		GameBoard& RefrenceGameBoard();
		protected:
			Sprite< -1 > move, grenade, flameThrower, check, hand, leftArrow, rightArrow, littleTarget, littleMove, throwGrenade;
			sf::IntRect actionBar;
			std::array< Sprite< -1 >*, 9 > uiElements;
			std::array< Sprite< -1 >*, 5 > actionBarSprites;
			int currentAction = 0;
			void DrawGUI( sf::RenderWindow& window );
			template< typename ORDER_TYPE >
			bool UpdatePlayer();
			void PlayerGiveOrdersStage( sf::RenderWindow& window );
			std::vector< Updator* > entities;
			std::optional< entt::entity > idOfSelectedSquaddie;
			GameBoard gameBoard;
			entt::registry& registry;
			void InitilizeUIComponents();
	};
}
#endif
