#include "TileTagging.hpp"
#ifndef UPDATOR_HEADER_HPP
#define UPDATOR_HEADER_HPP
namespace StrategyGoo
{
	struct Updator : public WorldPosition
	{
		Updator( entt::registry& registry_, BoardPosition start, GameBoard* board_,
			size_t tileWidth, size_t tileHeight, std::string spriteName );

		template< typename REFRENCE_TYPE, typename CLASS_TYPE >
		void InitilizeRefrences( CLASS_TYPE& self ) {
			registry.emplace< REFRENCE_TYPE >( id, self );
			AddEntityToTile< REFRENCE_TYPE >( registry, self, RefrenceBoardPosition(), board );
		}


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

	//template void Updator::InitilizeRefrences< UpdatorRefrence >();
}
#endif
