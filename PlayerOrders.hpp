#include "Goo.hpp"

#ifndef PLAYER_ORDERS_HEADER_HPP
#define PLAYER_ORDERS_HEADER_HPP
namespace BioGooContainmentSquad
{
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
		const static size_t MAX_DISTANCE_CONSTANT = 5;
	};

	struct FlamethrowerOrder;

	template<>
	struct PlayerOrderMaxDistance< FlamethrowerOrder > {
		const static size_t MAX_DISTANCE_CONSTANT = 3;
	};

	struct FlamethrowerOrder : public PlayerOrder
	{
		using FlameSpriteType = std::vector< Sprite >;
		Direction direction;
		FlamethrowerOrder( Squaddie& squaddie, sf::Vector2i mousePosition );
		bool Tick( Squaddie& squaddie, entt::registry& registry ) override;
		void KillGoo( Squaddie& squaddie, entt::registry& registry );
		void CreateFlameSprite( Squaddie& squaddie, entt::registry& registry );
		size_t maxFlameLength = PlayerOrderMaxDistance< FlamethrowerOrder >::MAX_DISTANCE_CONSTANT;
		size_t flameTicks = 0;
		size_t flameTicksPerFlame = 3;
		size_t maxFlameTicks = maxFlameLength * flameTicksPerFlame;
		bool killedGoo = false;
		entt::entity flameID;
	};
}
#endif
