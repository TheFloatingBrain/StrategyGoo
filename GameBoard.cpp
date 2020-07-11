#include "GameBoard.hpp"

namespace StrategyGoo
{
	Tile::Tile( entt::registry& registry_, size_t x_, size_t y_, size_t width, size_t height ) : 
			registry( registry_ ), x( x_ ), y( y_ ), WIDTH_CONSTANT( width ), HEIGHT_CONSTANT( height )
	{
		id = registry.create();
		sprite = &registry.emplace< Sprite >( id, "FloorTile0" );
		( **sprite ).setPosition( ( float ) x * TILE_WIDTH_CONSTANT, ( float ) y * HEIGHT_CONSTANT );
		registry.emplace< TileRefrence >( id, ( *this ) );
	}

	sf::Vector2f Tile::ToWorldPosition()
	{
		return sf::Vector2f( ( float ) x * TILE_WIDTH_CONSTANT, 
				( float ) y * TILE_HEIGHT_CONSTANT );
	}

	BoardPosition Tile::ObtainBoardPosition() {
		return BoardPosition{ x, y };
	}

	size_t Tile::GetX() {
		return x;
	}
	size_t Tile::GetX() {
		return y;
	}
	const size_t Tile::GetTileWidthConstant() {
		return WIDTH_CONSTANT;
	}
	const size_t Tile::GetTileHeightConstant() {
		return HEIGHT_CONSTANT;
	}


	GameBoard::GameBoard( entt::registry& registry_, size_t width_, size_t height_, 
				size_t tileWidth, size_t tileHeight ) :
			registry( registry_ ), width( width_ ), height( height_ ), 
			BOARD_TILE_WIDTH_CONSTANT( width ), BOARD_TILE_HEIGHT_CONSTANT( height ) {
		InitializeTiles();
	}
	GameBoard::Row GameBoard::operator[]( size_t index ) {
		return At( index );
	}
	GameBoard::Row GameBoard::At( size_t index ) {
		return Row{ &tiles, width, index };
	}

	void GameBoard::InitializeTiles()
	{
		for( size_t x = 0; x < width; ++x )
		{
			for( size_t y = 0; y < height; ++y ) {
				tiles.push_back( Tile{ registry, x, y,
					BOARD_TILE_WIDTH_CONSTANT, BOARD_TILE_HEIGHT_CONSTANT } );
			}
		}
	}

	size_t GameBoard::GetWidth() {
		return width;
	}
	size_t GameBoard::GetHeight() {
		return height;
	}

	const size_t GameBoard::GetTileWidthConstant() {
		return BOARD_TILE_WIDTH_CONSTANT;
	}
	const size_t GameBoard::GetTileHeightConstant() {
		return BOARD_TILE_HEIGHT_CONSTANT;
	}


	GameBoard::Row::Row( std::vector< Tile >* tiles_, size_t width, size_t row ) : 
		ROW_CONSTANT( row ), WIDTH_CONSTANT( width ), tiles( tiles_ ) {}

	Tile& GameBoard::Row::At( size_t index ) {
		return tiles->at( ( ROW_CONSTANT * WIDTH_CONSTANT ) + index );
	}

	Tile& GameBoard::Row::operator[]( size_t index ) {
		return At( index );
	}
}
