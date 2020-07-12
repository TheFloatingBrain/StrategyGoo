#include "Goo.hpp"
#include <iostream>

namespace StrategyGoo
{
	Goo::Goo( entt::registry& registry_, BoardPosition start, GameBoard* board_, size_t tileWidth, size_t tileHeight ) :
			Updator( registry_, start, board_, tileWidth, tileHeight, "CubeGoo1" ) {
		InitilizeRefrences< GooRefrence, Goo >( *this );
		//TODO after debugging.//
		//RefrenceSprite().SetActive( false );
	}

	Goo::GooComponent::GooComponent( entt::registry& registry_, BoardPosition start, GameBoard* board_,
			size_t tileWidth, size_t tileHeight, Goo* parent_ ) : 
			Updator( registry_, start, board_, tileWidth, tileHeight, "CubeGoo1" ), parent( parent_ ) {
		InitilizeRefrences< Goo::GooComponentRefrence, Goo::GooComponent >( *this );
	}

	Goo::GooComponent& Goo::AddGoo( BoardPosition location )
	{
		GooComponent* newGoo = new GooComponent( 
			registry, location, board, ( size_t ) TILE_WIDTH_CONSTANT, ( size_t ) TILE_HEIGHT_CONSTANT, this );
		goo.push_back( newGoo );
		return *newGoo;
	}

	Goo::EmptySquareLocalType Goo::FindEmptyAround( GooComponent& toLookAround, size_t spreadReach )
	{
		auto position = toLookAround.RefrenceBoardPosition();
		GameBoard& board = *toLookAround.GetBoard();
		EmptySquareLocalType emptySquares;

		auto checkForGoo = [ & ]( Tile& tile ) {
			return toLookAround.RefrenceRegistry().has<
					GooComponentRefrence >( tile.GetID() );
		};

		for( size_t y = ( ( ( size_t ) position.y >= spreadReach ) ? position.y - spreadReach : 0 );
				y <= ( position.y + spreadReach ) && y < board.GetHeight(); ++y )
		{
			for( size_t x = ( ( ( size_t ) position.x >= spreadReach ) ? position.x - spreadReach : 0 );
					x <= ( position.x + spreadReach ) && x < board.GetWidth(); ++x ) {
				if( checkForGoo( board[ x ][ y ] ) == false )
					emptySquares.push_back( BoardPosition( x, y ) );
			}
		}
		return emptySquares;
	}
	
	Goo::EmptySquareType Goo::FindEmptySquares( Goo& goo, size_t spreadReach )
	{
		Goo::EmptySquareType allEmptySquares;
		std::vector< Goo::GooComponent* >& allGoo = goo.GetGoo();
		for( Goo::GooComponent* currentGoo : allGoo ) {
			allEmptySquares.push_back( std::pair< Goo::GooComponent&, Goo::EmptySquareLocalType >( 
					*currentGoo, Goo::FindEmptyAround( *currentGoo, spreadReach ) ) );
		}
		return allEmptySquares;
	}

	std::vector< Goo::GooComponent* >& Goo::GetGoo() {
		return goo;
	}

}
