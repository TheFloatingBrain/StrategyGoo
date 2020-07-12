#include "VectorUtility.hpp"
#ifndef PLAYER_HEADER_HPP
namespace StrategyGoo
{
	struct Updator : public WorldPosition
	{
		Updator( entt::registry& registry_, GameBoard* board_, 
				size_t tileWidth, size_t tileHeight );
		using UpdatorRefrence = std::reference_wrapper< Updator >;
		virtual void Update() = 0;
		virtual void UpdateGraphics() {};
		BoardPosition& RefrenceBoardPosition();
		Sprite< 0 >& RefrenceSprite();
		entt::entity GetID();
		GameBoard* GetBoard();
		sf::Vector2f ToWorldPosition() override;
		protected: 
			entt::entity id;
			entt::registry& registry;
			GameBoard* board;
			const size_t ENTITY_TILE_WIDTH_CONSTANT;
			const size_t ENTITY_TILE_HEIGHT_CONSTANT;
	};

	struct Squaddie : public Updator
	{
		using SquaddieRefrence = std::reference_wrapper< Squaddie >;
		Squaddie( entt::registry& registry_, BoardPosition start, GameBoard* board_,
			size_t tileWidth, size_t tileHeight );

		void Update() override;
		void TickOrder();
		bool CheckSelect( entt::registry& registry, sf::RenderWindow& window );

		static std::pair< bool, std::optional< entt::entity > > SelectSquaddie( entt::registry& registry, sf::RenderWindow& window );
		static bool AddOrders( entt::registry& registry, entt::entity& id, sf::View& camera );
		static bool ExecuteOrders( entt::registry& registry );
		
		protected:
			float speed = 1.f / 5.f;

	};

	struct PlayerOrder {
		virtual bool Execute( Squaddie& squaddie ) = 0;
		virtual bool Tick( Squaddie& squaddie ) = 0;
	};

	struct MoveOrder : public PlayerOrder
	{
		BoardPosition from, to;
		MoveOrder( BoardPosition from_, BoardPosition to_ );
		bool Execute( Squaddie& squaddie );
		bool Tick( Squaddie& squaddie ) override;
	};
}
#endif
