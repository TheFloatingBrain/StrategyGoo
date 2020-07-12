#include "Goo.hpp"
namespace StrategyGoo
{
	Goo::Goo( entt::registry& registry_, BoardPosition start, GameBoard* board_, size_t tileWidth, size_t tileHeight ) :
			Updator( registry_, start, board_, tileWidth, tileHeight, "CubeGoo1" ) {
		InitilizeRefrences< GooRefrence, Goo >( *this );
	}
}
