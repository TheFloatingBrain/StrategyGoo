#include "TileTagging.hpp"
#ifndef ENTITY_HEADER_HPP
#define ENTITY_HEADER_HPP
namespace BioGooContainmentSquad
{
	struct Entity : public WorldPosition
	{
		Entity( entt::registry& registry_, BoardPosition start, GameBoard* board_,
			size_t tileWidth, size_t tileHeight, std::string spriteName );

		template< typename REFRENCE_TYPE, typename CLASS_TYPE >
		void InitilizeRefrences( CLASS_TYPE& self ) {
			registry.emplace< REFRENCE_TYPE >( id, self );
			AddEntityToTile< REFRENCE_TYPE >( registry, self, RefrenceBoardPosition(), board );
		}
		using UpdatorRefrence = std::reference_wrapper< Entity >;
		virtual void Update() = 0;
		virtual void UpdateGraphics() {};
		void TranslateToBoard( sf::Vector2i displacement );
		void TranslateToWorld( sf::Vector2f displacement );
		BoardPosition& RefrenceBoardPosition();
		Sprite& RefrenceSprite();
		entt::registry& RefrenceRegistry();
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
