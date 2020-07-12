#include "Updator.hpp"

namespace StrategyGoo
{
	Updator::Updator( entt::registry& registry_, GameBoard* board_,
		size_t tileWidth, size_t tileHeight ) : registry( registry_ ), board( board_ ),
		ENTITY_TILE_WIDTH_CONSTANT( tileWidth ), ENTITY_TILE_HEIGHT_CONSTANT( tileHeight ) {}

	BoardPosition& Updator::RefrenceBoardPosition() {
		return registry.get< BoardPosition >( id );
	}
	Sprite< 0 >& Updator::RefrenceSprite() {
		return registry.get< Sprite< 0 > >( id );
	}

	GameBoard* Updator::GetBoard() {
		return board;
	}
	entt::entity Updator::GetID() {
		return id;
	}

	sf::Vector2f Updator::ToWorldPosition() {
		auto position = RefrenceBoardPosition();
		return sf::Vector2f( ( float ) position.x * ENTITY_TILE_WIDTH_CONSTANT,
			( float ) position.y * ENTITY_TILE_WIDTH_CONSTANT );
	}
}
