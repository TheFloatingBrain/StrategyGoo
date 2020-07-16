#include "GameBoard.hpp"
#include <iostream>
namespace BioGooContainmentSquad
{
	Tile::Tile( entt::registry& registry_, size_t x_, size_t y_, size_t width, size_t height ) : 
			registry( registry_ ), x( x_ ), y( y_ ), WIDTH_CONSTANT( width ), HEIGHT_CONSTANT( height )
	{
		id = registry.create();
		registry.emplace< Sprite< 1 > >( id, "FloorTile0" );
		RefrenceSprite().RefrenceSprite().setPosition( ( float ) x * TILE_WIDTH_CONSTANT, ( float ) y * HEIGHT_CONSTANT );
		registry.emplace< TileRefrence >( id, ( *this ) );
	}

	entt::entity Tile::GetID() {
		return id;
	}

	sf::Vector2f Tile::ToWorldPosition()
	{
		return sf::Vector2f( ( float ) x * TILE_WIDTH_CONSTANT, 
				( float ) y * TILE_HEIGHT_CONSTANT );
	}

	BoardPosition Tile::ObtainBoardPosition() {
		return BoardPosition{ ( int ) x, ( int ) y };
	}

	size_t Tile::GetX() {
		return x;
	}
	size_t Tile::GetY() {
		return y;
	}

	Sprite< 1 >& Tile::RefrenceSprite() {
		return registry.get< Sprite< 1 > >( id );
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
			BOARD_TILE_WIDTH_CONSTANT( tileWidth ), BOARD_TILE_HEIGHT_CONSTANT( tileHeight ) {
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
		return tiles->at( ( index * WIDTH_CONSTANT ) + ROW_CONSTANT );
	}

	Tile& GameBoard::Row::operator[]( size_t index ) {
		return At( index );
	}

	BoardPosition GameBoard::ToBoardCoordinates( sf::Vector2f worldCoordinate ) {
		return BoardPosition{ ( int ) ( worldCoordinate.x / ( ( float ) BOARD_TILE_WIDTH_CONSTANT ) ),
				( int ) ( worldCoordinate.y / ( ( float ) BOARD_TILE_HEIGHT_CONSTANT ) ) };
	}

	BoardPosition GameBoard::ToBoardCoordinates( sf::Vector2i worldCoordinate ) {
		return BoardPosition{ ( int ) ( worldCoordinate.x / BOARD_TILE_WIDTH_CONSTANT ),
				( int ) ( worldCoordinate.y / BOARD_TILE_HEIGHT_CONSTANT ) };
	}

	sf::Vector2f GameBoard::ToWorldCoordinates( BoardPosition position ) {
		return sf::Vector2f{ ( float ) position.x * BOARD_TILE_WIDTH_CONSTANT, 
				( float ) position.y * BOARD_TILE_WIDTH_CONSTANT };
	}

}
