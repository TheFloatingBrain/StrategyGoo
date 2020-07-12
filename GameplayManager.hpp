#ifndef GAMEPLAY_MANAGER_HEADER_HPP
#define GAMEPLAY_MANAGER_HEADER_HPP
#include "Goo.hpp"

namespace StrategyGoo
{
	struct GameplayManager
	{
		GameplayManager( entt::registry& registry_ );
		void Update();
		void Render( sf::RenderWindow& window );
		Squaddie& CreateSquaddie( BoardPosition startingPosition );
		entt::registry& RefrenceRegistry();
		protected:
			template< typename ORDER_TYPE >
			void UpdatePlayer();
			void PlayerGiveOrdersStage( sf::RenderWindow& window );
			std::vector< Updator* > entities;
			std::optional< entt::entity > idOfSelectedSquaddie;
			GameBoard gameBoard;
			entt::registry& registry;
	};
}
#endif
