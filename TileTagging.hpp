#include "VectorUtility.hpp"

#ifndef TILE_TAGGING_HEADER_HPP
#define TILE_TAGGING_HEADER_HPP
namespace StrategyGoo
{
	template< typename REFRENCE_TYPE >
	bool AddEntityToTile( entt::registry& registry, REFRENCE_TYPE toRefrence, BoardPosition tile, GameBoard* board )
	{
		auto tileID = ( *board )[ tile.x ][ tile.y ].GetID();
		if( registry.has< REFRENCE_TYPE >( tileID ) == false ) {
			registry.emplace< REFRENCE_TYPE >( tileID, toRefrence );
			return true;
		}
		return false;
	}

	template< typename REFRENCE_TYPE >
	bool RemoveEntityFromTile( entt::registry& registry, BoardPosition tile, GameBoard* board )
	{
		auto tileID = ( *board )[ tile.x ][ tile.y ].GetID();
		if( registry.has< REFRENCE_TYPE >( tileID ) == true ) {
			registry.remove< REFRENCE_TYPE >( tileID );
			return true;
		}
		return false;
	}

	template< typename REFRENCE_TYPE >
	bool MoveEntity( entt::registry& registry, REFRENCE_TYPE toRefrence, 
			BoardPosition from, BoardPosition to, GameBoard* board ) {
		return ( AddEntityToTile( registry, toRefrence, to, board ) && 
				RemoveEntityFromTile< REFRENCE_TYPE >( registry, from, board ) );
	}
}
#endif
