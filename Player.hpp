#include "Updator.hpp"

#ifndef PLAYER_HEADER_HPP
#define PLAYER_HEADER_HPP
namespace StrategyGoo
{
	struct Squaddie : public Updator
	{
		using SquaddieRefrence = std::reference_wrapper< Squaddie >;
		Squaddie( entt::registry& registry_, BoardPosition start, GameBoard* board_,
			size_t tileWidth, size_t tileHeight );

		void Update() override;
		void TickOrder();
		bool CheckSelect( entt::registry& registry, sf::RenderWindow& window );

		float GetSpeed();
		void SetSpeed( float speed_ );

		static std::pair< bool, std::optional< entt::entity > > SelectSquaddie( entt::registry& registry, sf::RenderWindow& window );
		static bool AddOrders( entt::registry& registry, entt::entity& id, sf::View& camera );
		static bool ExecuteOrders( entt::registry& registry );
		
		Squaddie& ObtainThis();

		bool active = true;
		protected:
			float speed = 1.f;

	};

	template< typename PLAYER_ORDER_TYPE >
	struct PlayerOrderMaxDistance {
		const static size_t MAX_DISTANCE_CONSTANT = 0;
	};

	struct PlayerOrder {
		virtual bool Tick( Squaddie& squaddie, entt::registry& registry ) = 0;
	};

	struct MoveOrder : public PlayerOrder
	{
		BoardPosition from, to;
		MoveOrder( BoardPosition from_, BoardPosition to_ );
		bool Tick( Squaddie& squaddie, entt::registry& registry ) override;
	};

	template<>
	struct PlayerOrderMaxDistance< MoveOrder > {
		const static size_t MAX_DISTANCE_CONSTANT = 3;
	};

	struct ShootGrenadeOrder : public PlayerOrder
	{
		ShootGrenadeOrder( BoardPosition from_, BoardPosition to_ );
		bool Tick( Squaddie& squaddie, entt::registry& registry ) override;
		bool createdGrenade = false;
		bool detonatedGrenade = false;
		BoardPosition from, to;
		size_t explosionTime = 0;
		size_t maxExplosionTime = 5;
		bool killedGoo = false;
		entt::entity grenadeID, explosionID;
	};

	template<>
	struct PlayerOrderMaxDistance< ShootGrenadeOrder > {
		const static size_t MAX_DISTANCE_CONSTANT = 100;
	};
}
#endif
