#ifndef GAMEPLAY_MANAGER_HEADER_HPP
#define GAMEPLAY_MANAGER_HEADER_HPP
#include "GameBoard.hpp"
#include <math.h>
namespace StrategyGoo
{
	template< typename SCALER_TYPE = float >
	constexpr double Magnitude( sf::Vector2< SCALER_TYPE > of ) {
		return sqrt( ( of.x * of.x ) + ( of.y * of.y ) );
	}

	template< typename SCALER_0_TYPE, typename SCALER_1_TYPE >
	constexpr sf::Vector2< SCALER_0_TYPE > ConvertVector( sf::Vector2< SCALER_1_TYPE > vector ) {
		return sf::Vector2< SCALER_0_TYPE >{ ( SCALER_0_TYPE ) vector.x, ( SCALER_0_TYPE ) vector.y };
	}

	template< typename SCALER_TYPE >
	constexpr sf::Vector2< SCALER_TYPE > RectanglePosition( sf::Rect< SCALER_TYPE > from ) {
		return sf::Vector2< SCALER_TYPE >( from.left, from.top );
	}

	bool ComparePosition( sf::Vector2f first, sf::Vector2f second );

	template< typename SCALER_TYPE = float >
	struct ToUnitVector
	{
		sf::Vector2< SCALER_TYPE > result;
		ToUnitVector( sf::Vector2< SCALER_TYPE > vector ) {
			const double MAGNITUDE_CONSTANT = Magnitude( vector );
			result = sf::Vector2< SCALER_TYPE >( 
					vector.x / ( SCALER_TYPE ) MAGNITUDE_CONSTANT, vector.y / ( SCALER_TYPE ) MAGNITUDE_CONSTANT );
		}
		operator sf::Vector2< SCALER_TYPE >() {
			return result;
		}
	};

	struct Updator
	{
		using UpdatorRefrence = std::reference_wrapper< Updator >;
		virtual void Update() = 0;
		virtual void UpdateGraphics() {};
		entt::entity id;
	};

	struct Squaddie : public Updator, public WorldPosition
	{
		using SquaddieRefrence = std::reference_wrapper< Squaddie >;
		Squaddie( entt::registry& registry_, BoardPosition start, GameBoard* board_, 
				size_t tileWidth, size_t tileHeight );

		sf::Vector2f ToWorldPosition() override;
		//Anything that needs to be regularly updated.//
		void Update() override;
		//For animations between orders.//
		void TickOrder();
		bool CheckSelect( entt::registry& registry, sf::RenderWindow& window );
		
		BoardPosition& RefrenceBoardPosition();
		Sprite< 0 >& RefrenceSprite();
		GameBoard* board;
		const size_t ENTITY_TILE_WIDTH_CONSTANT;
		const size_t ENTITY_TILE_HEIGHT_CONSTANT;
		entt::registry& registry;

		static std::optional< entt::entity > SelectSquaddie( entt::registry& registry, sf::RenderWindow& window );
		static bool AddOrders( entt::registry& registry, entt::entity& id, sf::View& camera );
		static bool ExecuteOrders( entt::registry& registry );
	};

	struct PlayerOrder {
		virtual bool Tick( Squaddie& squaddie ) = 0;
	};

	struct MoveOrder : public PlayerOrder
	{
		BoardPosition from, to;
		MoveOrder( BoardPosition from_, BoardPosition to_ );
		bool Move( Squaddie& squaddie );
		bool Tick( Squaddie& squaddie ) override;
	};


	struct GameplayManager
	{
		GameplayManager( entt::registry& registry_ );
		void Update();
		void Render( sf::RenderWindow& window );
		Squaddie& CreateSquaddie( BoardPosition startingPosition );
		entt::registry& RefrenceRegistry();
		protected:
			void UpdatePlayer();
			std::vector< Updator* > entities;
			GameBoard gameBoard;
			entt::registry& registry;
	};
}
#endif
