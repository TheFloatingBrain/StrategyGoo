#include "Goo.hpp"
namespace StrategyGoo
{
	Goo::Goo( entt::registry& registry_, BoardPosition start, GameBoard* board_, size_t tileWidth, size_t tileHeight ) :
		Updator( registry_, board_, tileWidth, tileHeight )
	{
		id = registry.create();
		registry.emplace< BoardPosition >( id, start.x, start.y );
		registry.emplace< Sprite< 0 > >( id, "CubeGoo1" );
		RefrenceSprite().RefrenceSprite().setPosition( ToWorldPosition() );
		registry.emplace< GooRefrence >( id, *this );
		registry.emplace< Updator::UpdatorRefrence >( id, *this );
	}
}
