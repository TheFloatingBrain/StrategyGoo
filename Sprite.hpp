#ifndef SPRITE_HEADER_HPP
#define SPRITE_HEADER_HPP
#include <SFML/Graphics.hpp>
#include <string>
#include <array>
#include <tuple>
#include <algorithm>
#include <optional>

namespace StrategyGoo
{
	enum class Direction : unsigned short
	{
		NORTH = 0,
		SOUTH = 1,
		EAST = 2,
		WEST = 3,
		NORTH_EAST = 4,
		SOUTH_EAST = 5,
		NORTH_WEST = 6,
		SOUTH_WEST = 7
	};

	const size_t AMOUNT_OF_DIRECTIONS_CONSTANT = 8;

	const std::array< const std::string, 
			AMOUNT_OF_DIRECTIONS_CONSTANT > STRING_DIRECTIONS_ABBREVIATIONS_CONSTANT {
					"N", "S", "E", "W", "NE", "SE", "NW", "SW"
			};


	const Direction DEFAULT_DIRECTION = Direction::NORTH;

	using FRAMES_TYPE = std::array< std::vector< sf::IntRect >,
		AMOUNT_OF_DIRECTIONS_CONSTANT >;

	using ANIMATION_TYPE = std::array< std::vector< size_t >, 
				AMOUNT_OF_DIRECTIONS_CONSTANT >;

	using LOADED_SPRITE_DATA_TYPE = std::tuple< std::string, FRAMES_TYPE, std::string, sf::IntRect >;

	const size_t LOADED_SPRITE_DATA_TYPE_SPRITE_NAME_CONSTANT = 0;
	const size_t LOADED_SPRITE_DATA_TYPE_SPRITE_FRAMES_CONSTANT = 1;
	const size_t LOADED_SPRITE_DATA_TYPE_SPRITE_JSON_DATA_CONSTANT = 2;
	const size_t LOADED_SPRITE_DATA_TYPE_SPRITE_INT_RECT_CONSTANT = 3;

	namespace Detail
	{
		extern sf::Texture* masterTexture;
		extern sf::Image* masterSpriteBuffer;
		extern bool masterTextureInitialized;
		extern std::vector< LOADED_SPRITE_DATA_TYPE > spritesLoaded;
	}

	const std::string DEFAULT_SPRITE_NAME_CONSTANT = "DEFAULT_SPRITE_NAME";
	const std::string DEFAULT_JSON_DATA_CONSTANT = "DEFAULT_JSON_DATA_NAME";

	struct Sprite
	{
		Sprite( std::string spriteName_, Direction currentDirection_ = DEFAULT_DIRECTION );

		void Draw( sf::RenderWindow& toRenderTo );

		void ChangeAnimation( size_t to );
		std::vector< sf::IntRect > ObtainFramesForDirection( Direction spriteDirection );
		std::vector< size_t > ObtainAnimationFramesForDirection( Direction spriteDirection );
		std::vector< sf::IntRect > ObtainIntRectAnimationFramesForDirection( size_t animation, Direction spriteDirection );
		std::vector< sf::IntRect > ObtainIntRectCurrentAnimationFramesForDirection( Direction spriteDirection );
		bool IsDirectionAvailible( Direction spriteDirection );
		sf::IntRect ObtainAnimationFrameBounds( size_t animation, Direction spriteDirection, size_t frame );
		sf::IntRect ObtainCurrentAnimationFrameBounds( Direction spriteDirection, size_t frame );
		sf::IntRect ObtainFrameBounds( Direction spriteDirection, size_t frame );
		size_t ObtainFramesForAnimation( Direction spriteDirection, size_t animation );
		size_t ObtainFramesForCurrentAnimation();
		void ModifyTexture( const sf::Texture& texture );
		void AddAnimation( ANIMATION_TYPE toAdd );
		size_t ObtainAmountOfAnimations();
		ANIMATION_TYPE ObtainAnimation( size_t animation );

		void SetSprite( sf::Sprite sprite_ );
		void SetCurrentDirection( Direction currentDirection_ );
		void SetFrames( FRAMES_TYPE frames_ );
		void SetAnimations( std::vector< ANIMATION_TYPE > animations_ );
		void SetCurrentFrame( size_t currentFrame_ );
		void SetActive( bool active_ );
		void SetJSONData( std::string jsonData_ );
		void SetPlaceInSpriteSheet( sf::IntRect placeInSpriteSheet_ );
		void SetAnimationActive( bool animationActive_ );

		std::string GetSpriteName();
		sf::Sprite GetSprite();
		FRAMES_TYPE GetFrames();
		std::vector< ANIMATION_TYPE > GetAnimations();
		Direction GetCurrentDirection();
		size_t GetCurrentFrame();
		size_t GetCurrentAnimation();
		bool GetActive();
		std::string GetJSONData();
		sf::IntRect GetPlaceInSpriteSheet();
		bool GetAnimationActive();

		static sf::IntRect LoadSpriteImage( std::string spriteName, sf::Texture* buffer = nullptr, 
				sf::Image* spriteBuffer = nullptr, std::vector< LOADED_SPRITE_DATA_TYPE >* loadedSprites = nullptr );
		static LOADED_SPRITE_DATA_TYPE LoadSprite( std::string spriteName, sf::Texture* buffer = nullptr, 
				sf::Image* spriteBuffer = nullptr, std::vector< LOADED_SPRITE_DATA_TYPE >* loadedSprites = nullptr );
		static std::optional< int > ObtainLoadedSpriteIndex( std::string spriteName, 
				std::vector< LOADED_SPRITE_DATA_TYPE >* loadedSprites = nullptr );
		static std::string LoadJSONData( std::string spriteName );
		void InitializeSprite( std::string spriteName_ );
		static FRAMES_TYPE FrameDataFromJSONMetaData( std::string metaData, std::string spriteName );

		operator sf::Sprite();
		sf::Sprite& operator*();
		sf::Sprite& RefrenceSprite();
		std::vector< ANIMATION_TYPE >& RefrenceAnimations();
		ANIMATION_TYPE& RefrenceAnimation( size_t animation );

		protected:
			void SetCurrentAnimation( size_t currentAnimation_ );
			std::string spriteName = DEFAULT_SPRITE_NAME_CONSTANT;
			sf::Sprite sprite;
			FRAMES_TYPE frames;
			Direction currentDirection;
			size_t currentFrame = 0;
			size_t currentAnimation = 0;
			sf::IntRect placeInSpriteSheet;
			std::vector< ANIMATION_TYPE > animations;
			bool active = true;
			bool animationActive = true;
			std::string jsonData = DEFAULT_JSON_DATA_CONSTANT;
			void Animate();
			void ConstructDefaultAnimation();
	};
}
#endif
