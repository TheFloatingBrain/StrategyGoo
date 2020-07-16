#include "Entity.hpp"

#ifndef SQUADDIE_HEADER_HPP
#define SQUADDIE_HEADER_HPP
namespace BioGooContainmentSquad
{
	struct Squaddie : public Entity
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
}
#endif
