#include "Sprite.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace StrategyGoo
{
	namespace Detail
	{
		sf::Texture* masterTexture = nullptr;
		sf::Image* masterSpriteBuffer = nullptr;
		bool masterTextureInitialized = false;
		std::vector< LOADED_SPRITE_DATA_TYPE > spritesLoaded;
	}
	Sprite::Sprite( std::string spriteName_, Direction currentDirection ) {
		std::cerr << "STUB\n";
	}
	Sprite::Sprite( const sf::Texture& alternativeTexture, FRAMES_TYPE frames ) {
		std::cerr << "STUB\n";
	}
	Sprite::Sprite( sf::Sprite spriteName_ ) {
		std::cerr << "STUB\n";
	}
	void Sprite::Draw( sf::RenderWindow& window ) {
		std::cerr << "STUB\n";
	}
	Sprite Sprite::Copy() {
		return Sprite{ "STUB" };
	}

	std::vector< sf::IntRect > Sprite::ObtainFramesForDirection( Direction spriteDirection ) {
		return frames[ ( unsigned short ) spriteDirection ];
	}
	std::vector< size_t > Sprite::ObtainAnimationFramesForDirection( Direction spriteDirection ) {
		return animations[ currentAnimation ][ ( unsigned short ) spriteDirection ];
	}
	sf::IntRect Sprite::ObtainAnimationFrameBounds( size_t animation, Direction spriteDirection, size_t frame ) {
		return frames[ ( unsigned short ) spriteDirection ][
			animations[ animation ][ ( unsigned short ) spriteDirection ][ frame ] ];
	}
	sf::IntRect Sprite::ObtainCurrentAnimationFrameBounds( Direction spriteDirection, size_t frame ) {
		return ObtainAnimationFrameBounds( currentAnimation, spriteDirection, frame );
	}
	std::vector< sf::IntRect > Sprite::ObtainIntRectAnimationFramesForDirection( size_t animation, Direction spriteDirection )
	{
		std::vector< sf::IntRect > framesForAnimation;
		const size_t AMOUNT_OF_FRAMES_CONSTANT = animations[ currentAnimation ][ 
				( unsigned short ) spriteDirection ].size();
		for( size_t i = 0; i < AMOUNT_OF_FRAMES_CONSTANT; ++i ) {
			framesForAnimation.push_back( ObtainAnimationFrameBounds( animation, spriteDirection, i ) );
		}
		return framesForAnimation;
	}
	std::vector< sf::IntRect > Sprite::ObtainIntRectCurrentAnimationFramesForDirection( Direction spriteDirection ) {
		return ObtainIntRectAnimationFramesForDirection( currentAnimation, spriteDirection );
	}

	sf::IntRect Sprite::ObtainFrameBounds( Direction spriteDirection, size_t frame ) {
		return frames[ ( unsigned short ) spriteDirection ][ frame ];
	}

	bool Sprite::IsDirectionAvailible( Direction spriteDirection ) {
		return frames[ ( unsigned short ) spriteDirection ].size() == 0;
	}
	void Sprite::ModifyTexture( const sf::Texture& texture ) {
		sprite.setTexture( texture );
	}
	void Sprite::AddAnimation( ANIMATION_TYPE toAdd ) {
		animations.push_back( toAdd );
	}
	size_t Sprite::ObtainAmountOfAnimations() {
		return animations.size();
	}
	ANIMATION_TYPE Sprite::ObtainAnimation( size_t animation ) {
		return animations[ animation ];
	}

	void Sprite::SetSprite( sf::Sprite sprite_ ) {
		sprite = sprite_;
	}
	void Sprite::SetCurrentDirection( Direction currentDirection_ ) {
		currentDirection = currentDirection_;
	}
	void Sprite::SetFrames( FRAMES_TYPE frames_ ) {
		frames = frames_;
	}
	void Sprite::SetAnimations( std::vector< ANIMATION_TYPE > animations_ ) {
		animations = animations_;
	}
	void Sprite::SetCurrentFrame( size_t currentFrame_ ) {
		currentFrame = currentFrame_;
	}
	void Sprite::SetCurrentAnimation( size_t currentAnimation_ ) {
		currentAnimation = currentAnimation_;
	}
	void Sprite::SetActive( bool active_ ) {
		active = active_;
	}
	void Sprite::SetJSONData( std::string jsonData_ ) {
		jsonData = jsonData_;
	}
	void Sprite::SetPlaceInSpriteSheet( sf::IntRect placeInSpriteSheet_ ) {
		placeInSpriteSheet = placeInSpriteSheet_;
	}


	std::string Sprite::GetSpriteName() {
		return spriteName;
	}
	sf::Sprite Sprite::GetSprite() {
		return sprite;
	}
	FRAMES_TYPE Sprite::GetFrames() {
		return frames;
	}
	std::vector< ANIMATION_TYPE > Sprite::GetAnimations() {
		return animations;
	}
	Direction Sprite::GetCurrentDirection() {
		return currentDirection;
	}
	size_t Sprite::GetCurrentFrame() {
		return currentFrame;
	}
	size_t Sprite::GetCurrentAnimation() {
		return currentAnimation;
	}
	bool Sprite::GetActive() {
		return active;
	}
	std::string Sprite::GetJSONData() {
		return jsonData;
	}
	sf::IntRect Sprite::GetPlaceInSpriteSheet() {
		return placeInSpriteSheet;
	}

	const std::string SPRITE_FILE_NAME_ROOT_CONSTANT = "sizeBaseSloped";

	const std::string ASSETS_FOLDER_CONSTANT = "Assets";

	std::optional< int > Sprite::ObtainLoadedSpriteIndex( std::string spriteName, 
			std::vector< LOADED_SPRITE_DATA_TYPE >* loadedSprites )
	{
		const size_t LOADED_SPRITES_SIZE_CONSTANT = loadedSprites->size();
		for( size_t i = 0; i < LOADED_SPRITES_SIZE_CONSTANT; ++i  ) {
			if( std::get< 0 >( loadedSprites->at( i ) ) == spriteName )
				return i;
		}
		return std::nullopt;
	}

	sf::IntRect Sprite::LoadSpriteImage( std::string spriteName, sf::Texture* buffer, 
			sf::Image* spriteBuffer, std::vector< LOADED_SPRITE_DATA_TYPE >* loadedSprites )
	{
		if( buffer == nullptr )
		{
			if( Detail::masterTextureInitialized == false )
			{
				Detail::masterTexture = new sf::Texture;
				Detail::masterSpriteBuffer = new sf::Image;
				Detail::masterTextureInitialized = true;
			}
			buffer = Detail::masterTexture;
			spriteBuffer = Detail::masterSpriteBuffer;
		}
		sf::Image newImage, intermediateImage;
		newImage.loadFromFile( spriteName + "/" + SPRITE_FILE_NAME_ROOT_CONSTANT + ".png" );
		const size_t Y_AXIS_CONSTANT = std::max( spriteBuffer->getSize().y, newImage.getSize().y );
		const size_t ORIGINAL_X_SIZE_CONSTANT = spriteBuffer->getSize().x;
		//See https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1Image.php#ad9562b126fc8d5efcf608166992865c7 //
		intermediateImage.loadFromMemory( spriteBuffer->getPixelsPtr(),
			( spriteBuffer->getSize().x + newImage.getSize().x ) * ( Y_AXIS_CONSTANT ) * 4 );
		for( size_t x = 0; x < newImage.getSize().x; ++x ) {
			for( size_t y = 0; y < Y_AXIS_CONSTANT; ++y )
				intermediateImage.setPixel( x, y, newImage.getPixel( x, y ) );
		}
		spriteBuffer->loadFromMemory( intermediateImage.getPixelsPtr(), newImage.getSize().x * Y_AXIS_CONSTANT * 4 );
		return sf::IntRect{ ( int ) spriteBuffer->getSize().x, 0, ( int ) ORIGINAL_X_SIZE_CONSTANT, ( int ) Y_AXIS_CONSTANT };
	}

	std::string Sprite::LoadJSONData( std::string spriteName )
	{
		const std::string JSON_FILE_NAME_CONSTANT = ( ( ASSETS_FOLDER_CONSTANT + "/" ) +
			spriteName + ( "/" + ( SPRITE_FILE_NAME_ROOT_CONSTANT + ".json" ) ) );
		std::ifstream jsonFile( JSON_FILE_NAME_CONSTANT );
		std::stringstream jsonDataStream;
		std::string line;
		if( jsonFile.is_open() )
		{
			while( std::getline( jsonFile, line ) )
				jsonDataStream << line << "\n";
			jsonFile.close();
		}
		else {
			std::cerr << "StrategyGoo::Sprite::LoadSprite( std::string ) : void::Error: failed to read file " <<
					JSON_FILE_NAME_CONSTANT << "\n";
		}
		return jsonDataStream.str();
	}

	LOADED_SPRITE_DATA_TYPE Sprite::LoadSprite( std::string spriteName, sf::Texture* buffer, 
			sf::Image* spriteBuffer, std::vector< LOADED_SPRITE_DATA_TYPE >* loadedSprites )
	{
		const auto SPRITE_INDEX_CONSTANT = ObtainLoadedSpriteIndex( spriteName, loadedSprites );
		if( SPRITE_INDEX_CONSTANT.has_value() == false )
		{
			LOADED_SPRITE_DATA_TYPE data;
			std::get< LOADED_SPRITE_DATA_TYPE_SPRITE_NAME_CONSTANT >( data ) = spriteName;
			std::get< LOADED_SPRITE_DATA_TYPE_SPRITE_JSON_DATA_CONSTANT >( 
						data ) = LoadJSONData( spriteName );
			std::get< LOADED_SPRITE_DATA_TYPE_SPRITE_FRAMES_CONSTANT >( 
						data ) = FrameDataFromJSONMetaData( std::get< 2 >( data ), spriteName );
			std::get< LOADED_SPRITE_DATA_TYPE_SPRITE_INT_RECT_CONSTANT >( 
						data ) = LoadSpriteImage( spriteName, buffer, spriteBuffer, loadedSprites );
			return data;
		}
		return loadedSprites->at( SPRITE_INDEX_CONSTANT.value() );
	}
	
	void Sprite::InitializeSprite( std::string spriteName_ ) {
		std::cerr << "STUB\n";
	}
	void Sprite::InitializeSprite( const sf::Texture& alternativeTexture, FRAMES_TYPE frames ) {
		std::cerr << "STUB\n";
	}
	void Sprite::InitializeSprite( const sf::Texture& alternativeTexture, std::string metaData ) {
		std::cerr << "STUB\n";
	}

	const std::string SPRITE_FOLDER_CONSTANT = ( "/" + ( SPRITE_FILE_NAME_ROOT_CONSTANT + "/" ) );

	const std::string SPRITE_PREFIX_FOR_DIRECTION = "_Slope_";

	const size_t SPRITE_SHEET_PACKER_MAX_POWER_OF_10_CONSTANT = 2;

	FRAMES_TYPE Sprite::FrameDataFromJSONMetaData( std::string metaData, std::string spriteName )
	{
		FRAMES_TYPE frames;
		auto json = nlohmann::json{ metaData };
		const std::string SPRITE_ROOT_NAME_CONSTANT = ( spriteName + ( SPRITE_FOLDER_CONSTANT + (
				spriteName + SPRITE_PREFIX_FOR_DIRECTION ) ) );
		std::stringstream frameStream;
		auto makeFrameString = [&]( size_t frame )
		{
			frameStream.str( std::string{} );
			frameStream << "_" << std::setfill( '0' ) << std::setw( SPRITE_SHEET_PACKER_MAX_POWER_OF_10_CONSTANT ) << frame;
			return frameStream.str();
		};
		auto jsonDataToFrame = [ & ]( nlohmann::json& jsonData )
		{
			auto frameData = jsonData[ "frame" ];
			return sf::IntRect{ frameData[ "x" ], frameData[ "y" ],
				frameData[ "width" ], frameData[ "height" ] };
		};
		std::cout << json[ "Squaddie/sizeBaseSloped/Squaddie_Slope_E" ] << "\n";
		for( size_t i = 0; i < AMOUNT_OF_DIRECTIONS_CONSTANT; ++i )
		{
			const std::string DIRECTION_CONSTANT = ( SPRITE_ROOT_NAME_CONSTANT + 
					STRING_DIRECTIONS_ABBREVIATIONS_CONSTANT[ i ] );
			if( json.contains( DIRECTION_CONSTANT + makeFrameString( 0 ) ) == true )
			{
				std::vector< sf::IntRect > frameBuffer;
				for( size_t j = 0; json.contains( 
						DIRECTION_CONSTANT + makeFrameString( j ) ) == true; ++j )
				{
					frameBuffer.push_back( jsonDataToFrame( json[ ( DIRECTION_CONSTANT + makeFrameString( j ) ) ] ) );
				}
				frames[ i ].insert( frames[ i ].end(), frameBuffer.begin(), frameBuffer.end() );
			}
			else if( json.contains( DIRECTION_CONSTANT ) == true )
				frames[ i ].push_back( jsonDataToFrame( json[ DIRECTION_CONSTANT ] ) );
			else
			{
				std::cerr << "StrategyGoo::Sprite::InitializeSprite( std::string ) : "
						"void::Error: Failed to load frames for \"" <<
						STRING_DIRECTIONS_ABBREVIATIONS_CONSTANT[ i ] << "\" direction.\n";
			}
		}
		return frames;
	}

	Sprite::operator sf::Sprite() {
		return sprite;
	}
	sf::Sprite& Sprite::operator*() {
		return RefrenceSprite();
	}
	sf::Sprite& Sprite::RefrenceSprite() {
		return sprite;
	}
	std::vector< ANIMATION_TYPE >& Sprite::RefrenceAnimations() {
		return animations;
	}
	ANIMATION_TYPE& Sprite::RefrenceAnimation( size_t animation ) {
		return animations[ animation ];
	}
}
