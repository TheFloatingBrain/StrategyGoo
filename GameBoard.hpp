#ifndef GAME_BOARD_HEADER_HPP
#define GAME_BOARD_HEADER_HPP
#include <entt/entt.hpp>
#include "Sprite.hpp"

namespace StrategyGoo
{
	const size_t TILE_WIDTH_CONSTANT = 64;
	const size_t TILE_HEIGHT_CONSTANT = 64;

	using BoardPosition = sf::Vector2i;

	struct WorldPosition {
		virtual sf::Vector2f ToWorldPosition() = 0;
	};

	struct Tile : WorldPosition
	{
		using TileRefrence = std::reference_wrapper< Tile >;
		explicit Tile( entt::registry& registry_, size_t x_, size_t y_, 
				size_t width = TILE_WIDTH_CONSTANT, size_t height = TILE_HEIGHT_CONSTANT );

		sf::Vector2f ToWorldPosition() override;

		BoardPosition ObtainBoardPosition();

		size_t GetX();
		size_t GetY();
		const size_t GetTileWidthConstant();
		const size_t GetTileHeightConstant();

		Sprite< 1 >& RefrenceSprite();

		entt::entity GetID();

		protected: 
			
			size_t x, y;
			entt::entity id;
			entt::registry& registry;
			const size_t WIDTH_CONSTANT;
			const size_t HEIGHT_CONSTANT;
	};

	struct GameBoard
	{
		explicit GameBoard( entt::registry& registry_, size_t width_, size_t height_, 
				size_t tileWidth = TILE_WIDTH_CONSTANT, size_t tileHeight = TILE_HEIGHT_CONSTANT );

		struct Row
		{
			explicit Row( std::vector< Tile >* tiles_, size_t width, size_t row );
			Tile& operator[]( size_t index );
			Tile& At( size_t index );
			protected: 
				const size_t ROW_CONSTANT;
				const size_t WIDTH_CONSTANT;
				std::vector< Tile >* tiles;
		};

		size_t GetWidth();
		size_t GetHeight();
		const size_t GetTileWidthConstant();
		const size_t GetTileHeightConstant();

		Row operator[]( size_t index );
		Row At( size_t index );

		BoardPosition ToBoardCoordinates( sf::Vector2f worldCoordinate );
		BoardPosition ToBoardCoordinates( sf::Vector2i worldCoordinate );
		
		sf::Vector2f ToWorldCoordinates( BoardPosition position );

		protected: 
			void InitializeTiles();
			std::vector< Tile > tiles;
			size_t width, height;
			entt::registry& registry;
			const size_t BOARD_TILE_WIDTH_CONSTANT;
			const size_t BOARD_TILE_HEIGHT_CONSTANT;
	};
}
#endif
