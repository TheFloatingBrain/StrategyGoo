#ifndef GAMEPLAY_MANAGER_HEADER_HPP
#define GAMEPLAY_MANAGER_HEADER_HPP
#include "GameBoard.hpp"
#include <queue>
namespace StrategyGoo
{

	struct Updator
	{
		using UpdatorRefrence = std::reference_wrapper< Updator >;
		virtual void Update() = 0;
		virtual void Render() {}
	};

	struct PlayerUnit;
	
	struct PlayerOrder {
		virtual void Execute( PlayerUnit* player ) = 0;
	};

	struct PlayerUnit : public WorldPosition
	{
		using PlayerUnitRefrence = std::reference_wrapper< PlayerUnit >;
		std::queue< PlayerOrder* > orders;
		template< typename ORDER_TYPE, typename... ARGUMENT_TYPES >
		void AddOrder( ARGUMENT_TYPES... arguments ) {
			orders.push( new OREDER_TYPE( std::forward< ARGUMENT_TYPES > arguments )... );
		}
		void RemoveOrder() {
			delete orders.front();
;			orders.pop();
		}
		bool Done() {
			return orders.empty();
		}

		sf::Vector2f ToWorldPosition() override {
			return ( *board )[ position->x ][ position->y ].ToWorldPosition();
		}

		bool CheckSelect( sf::Vector2f cursorPosition, sf::Vector2f cameraPosition )
		{
			sf::IntRect box = sprite->ObtainCurrentAnimationFrameBounds( 
					sprite->GetCurrentDirection(), sprite->GetCurrentFrame() );
			auto worldPosition = ToWorldPosition();
			sf::FloatRect checkMouse = sf::FloatRect{ worldPosition.x,
					worldPosition.y, ( float ) box.width, ( float ) box.height };
			if( checkMouse.contains( ( cursorPosition + cameraPosition ) ) )
				selected = true;
			return selected;
		}
		//Plays order animations.//
		virtual bool TickOrder() = 0;
		BoardPosition* position;
		Sprite* sprite;
		GameBoard* board;
		bool selected = false;
	};


	struct TestEntity : public Updator, public PlayerUnit
	{
		using TestEntityRefrence = std::reference_wrapper< TestEntity >;
		TestEntity( entt::registry& registry_, BoardPosition start, size_t tileWidth, size_t tileHeight ) : 
				registry( registry_ ), ENTITY_TILE_WIDTH_CONSTANT( tileWidth ), ENTITY_TILE_HEIGHT_CONSTANT( tileHeight )
		{
			id = registry.create();
			position = &registry.emplace< BoardPosition >( id, start.x, start.y );
			sprite = &registry.emplace< Sprite >( id, "Squaddie" );
			registry.emplace< TestEntityRefrence >( id, *this );
			registry.emplace< Updator::UpdatorRefrence >( id, *this );
			registry.emplace< PlayerUnit::PlayerUnitRefrence >( id, *this );
		}
		//Anything that needs to be regularly updated.//
		void Update() override {
		}
		//For animations between orders.//
		void TickOrder() override {
			
		}
		const size_t ENTITY_TILE_WIDTH_CONSTANT;
		const size_t ENTITY_TILE_HEIGHT_CONSTANT;
		entt::entity id;
		entt::registry& registry;
	};

	struct GameplayManager
	{
		GameplayManager();
		void Update();
		void Render();
		protected: 
			GameBoard gameBoard;
			entt::registry registry;
	};
}
#endif
