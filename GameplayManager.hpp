#ifndef GAMEPLAY_MANAGER_HEADER_HPP
#define GAMEPLAY_MANAGER_HEADER_HPP
#include "Goo.hpp"

namespace StrategyGoo
{
	struct GameplayManager
	{
		GameplayManager( entt::registry& registry_, size_t width = 64, size_t height = 64 );

		enum class PlayerAction : size_t
		{
			GRENADE = 0,
			FLAME_THROWER = 1,
			MOVE = 2,
			PICK_UP = 3,
			END_TURN = 4,
			NONE = 5
		};

		enum class StagesOfPlay : size_t
		{
			PLAYER_GIVE_ORDERS_STAGE = 0, 
			PLAYER_EXECUTE_ORDERS_STAGE = 1, 
			SLIME_MOVE_STAGE = 2, 
			PLAYER_DAMAGE_STAGE = 3
		};
		StagesOfPlay GetGameState();
		void Update( sf::RenderWindow& window );
		void Render( sf::RenderWindow& window );
		template< typename ENTITY_TYPE >
		ENTITY_TYPE& CreateEntity( BoardPosition startingPosition );
		entt::registry& RefrenceRegistry();
		GameBoard& RefrenceGameBoard();

		int gameStatus = 0;
		protected:
			size_t deadSquaddieCount = 0;
			Sprite< -1 > move, grenade, flameThrower, check, hand, leftArrow, 
					rightArrow, littleTarget, littleMove, throwGrenade, defaultCursor, 
					cursorSprite, selectionSquare;
			StagesOfPlay gameState = StagesOfPlay::PLAYER_GIVE_ORDERS_STAGE;
			sf::IntRect actionBar;
			std::array< Sprite< -1 >*, 9 > uiElements;
			std::array< Sprite< -1 >*, 6 > actionBarSprites;
			PlayerAction currentAction = PlayerAction::NONE;
			void DrawGUI( sf::RenderWindow& window );
			template< typename ORDER_TYPE >
			bool UpdatePlayer();
			void PlayerGiveOrdersStage( sf::RenderWindow& window );
			void SlimeMove( sf::RenderWindow& window );
			std::vector< Updator* > entities;
			std::optional< entt::entity > idOfSelectedSquaddie;
			GameBoard gameBoard;
			entt::registry& registry;
			void InitilizeUIComponents();
	};
}
#endif
