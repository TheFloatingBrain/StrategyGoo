#include "Entity.hpp"

namespace BioGooContainmentSquad
{
	Entity::Entity( entt::registry& registry_, BoardPosition start, GameBoard* board_,
		size_t tileWidth, size_t tileHeight, std::string spriteName ) : registry( registry_ ), board( board_ ),
		ENTITY_TILE_WIDTH_CONSTANT( tileWidth ), ENTITY_TILE_HEIGHT_CONSTANT( tileHeight )
	{
		id = registry.create();
		registry.emplace< BoardPosition >( id, start.x, start.y );
		registry.emplace< Sprite >( id, spriteName );
		RefrenceSprite().SetPosition( ToWorldPosition() );
		registry.emplace< UpdatorRefrence >( id, *this );
	}

	BoardPosition& Entity::RefrenceBoardPosition() {
		return registry.get< BoardPosition >( id );
	}
	
	Sprite& Entity::RefrenceSprite() {
		return registry.get< Sprite >( id );
	}
	entt::registry& Entity::RefrenceRegistry() {
		return registry;
	}
	GameBoard* Entity::GetBoard() {
		return board;
	}
	entt::entity Entity::GetID() {
		return id;
	}
	sf::Vector2f Entity::ToWorldPosition() {
		auto position = RefrenceBoardPosition();
		return sf::Vector2f( ( float ) position.x * ENTITY_TILE_WIDTH_CONSTANT,
			( float ) position.y * ENTITY_TILE_WIDTH_CONSTANT );
	}
	void Entity::TranslateToBoard( sf::Vector2i displacement )
	{
		auto& position = RefrenceBoardPosition();
	}
	void Entity::TranslateToWorld( sf::Vector2f displacement )
	{

	}

}
