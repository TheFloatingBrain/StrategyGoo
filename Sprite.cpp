#include "Sprite.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <string_view>

namespace StrategyGoo
{
	namespace Detail
	{
		sf::Texture* masterTexture = nullptr;
		sf::Image* masterSpriteBuffer = nullptr;
		bool masterTextureInitialized = false;
		std::vector< LOADED_SPRITE_DATA_TYPE > spritesLoaded;
	}

	template< int LAYER_CONSTANT >
	Sprite< LAYER_CONSTANT >::Sprite( std::string spriteName_, Direction currentDirection_ ) : currentDirection( currentDirection_ ), currentFrame( 0 )
	{
		InitializeSprite( spriteName_ );
		ConstructDefaultAnimation();
		sprite = sf::Sprite( *Detail::masterTexture, frames[ ( unsigned short ) currentDirection ][ 0 ] );
		sprite.setTextureRect( frames[ 0 ][ 0 ] );
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::ConstructDefaultAnimation()
	{
		ANIMATION_TYPE newAnimation;
		for( size_t i = 0; i < frames.size(); ++i )
		{
			const size_t SIZE_OF_FRAMES_FOR_DIRECTION_CONSTANT = frames[ i ].size();
			for( size_t j = 0; j < SIZE_OF_FRAMES_FOR_DIRECTION_CONSTANT; ++j )
				newAnimation[ i ].push_back( j );
		}
		animations.push_back( newAnimation );
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::Animate()
	{
		bool animate = animationActive;
		if( animate == true && animationRate != 1.f )
		{
			animate = false;
			if( animationRateClock.getElapsedTime().asSeconds() > animationRate ) {
				animate = true;
				animationRateClock.restart();
			}
		}
		if( animate == true )
		{
			currentFrame = ( ( currentFrame < ( ObtainFramesForCurrentAnimation() - 1 ) ) ? currentFrame + 1 : 0 );
			auto relativeFrame = ObtainCurrentAnimationFrameBounds( currentDirection, currentFrame );
			relativeFrame.left += placeInSpriteSheet.left;
			relativeFrame.top += placeInSpriteSheet.top;
			sprite.setTextureRect( relativeFrame );
		}
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::Draw( sf::RenderWindow& toRenderTo )
	{
		if( active )
			toRenderTo.draw( sprite );
		Animate();
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::ChangeAnimation( size_t to ) {
		currentAnimation = to;
		//currentFrame = 0;
	}

	template< int LAYER_CONSTANT >
	std::vector< sf::IntRect > Sprite< LAYER_CONSTANT >::ObtainFramesForDirection( Direction spriteDirection ) {
		return frames[ ( unsigned short ) spriteDirection ];
	}

	template< int LAYER_CONSTANT >
	std::vector< size_t > Sprite< LAYER_CONSTANT >::ObtainAnimationFramesForDirection( Direction spriteDirection ) {
		return animations[ currentAnimation ][ ( unsigned short ) spriteDirection ];
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetAnimationRate( float animationRate_ ) {
		animationRate = animationRate_;
	}
	template< int LAYER_CONSTANT >
	int Sprite< LAYER_CONSTANT >::GetLayerConstant() {
		return LAYER_CONSTANT;
	}

	template< int LAYER_CONSTANT >
	sf::IntRect Sprite< LAYER_CONSTANT >::ObtainAnimationFrameBounds( size_t animation, Direction spriteDirection, size_t frame ) {
		/*std::cerr << "-------\nSprite Direction " << ( unsigned int ) spriteDirection << " frame size " << frames.size() << "\n";
		std::cerr << "Animation " << animation << " animations size " << animations.size() << "\n";
		std::cerr << "Sprite Direction " << ( unsigned int ) spriteDirection << " animation size " << animations[ animation ].size() << "\n";
		std::cerr << "Sprite Frame " << frame << " animation direction size " << animations[ animation ][ ( unsigned short ) spriteDirection ].size() << "+++++\n";
		*/return frames[ ( unsigned short ) spriteDirection ][
				animations[ animation ][ ( unsigned short ) spriteDirection ][ frame ] ];
	}

	template< int LAYER_CONSTANT >
	sf::IntRect Sprite< LAYER_CONSTANT >::ObtainCurrentAnimationFrameBounds( Direction spriteDirection, size_t frame ) {
		return ObtainAnimationFrameBounds( currentAnimation, spriteDirection, frame );
	}

	template< int LAYER_CONSTANT >
	size_t Sprite< LAYER_CONSTANT >::ObtainFramesForAnimation( Direction spriteDirection, size_t animation ) {
		return animations[ animation ][ ( unsigned short ) spriteDirection ].size();
	}

	template< int LAYER_CONSTANT >
	size_t Sprite< LAYER_CONSTANT >::ObtainFramesForCurrentAnimation() {
		return ObtainFramesForAnimation( currentDirection, currentAnimation );
	}

	template< int LAYER_CONSTANT >
	std::vector< sf::IntRect > Sprite< LAYER_CONSTANT >::ObtainIntRectAnimationFramesForDirection( size_t animation, Direction spriteDirection )
	{
		std::vector< sf::IntRect > framesForAnimation;
		const size_t AMOUNT_OF_FRAMES_CONSTANT = animations[ currentAnimation ][ 
				( unsigned short ) spriteDirection ].size();
		for( size_t i = 0; i < AMOUNT_OF_FRAMES_CONSTANT; ++i ) {
			framesForAnimation.push_back( ObtainAnimationFrameBounds( animation, spriteDirection, i ) );
		}
		return framesForAnimation;
	}

	template< int LAYER_CONSTANT >
	std::vector< sf::IntRect > Sprite< LAYER_CONSTANT >::ObtainIntRectCurrentAnimationFramesForDirection( Direction spriteDirection ) {
		return ObtainIntRectAnimationFramesForDirection( currentAnimation, spriteDirection );
	}

	template< int LAYER_CONSTANT >
	sf::IntRect Sprite< LAYER_CONSTANT >::ObtainFrameBounds( Direction spriteDirection, size_t frame ) {
		return frames[ ( unsigned short ) spriteDirection ][ frame ];
	}

	template< int LAYER_CONSTANT >
	bool Sprite< LAYER_CONSTANT >::IsDirectionAvailible( Direction spriteDirection ) {
		return frames[ ( unsigned short ) spriteDirection ].size() == 0;
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::ModifyTexture( const sf::Texture& texture ) {
		sprite.setTexture( texture );
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::AddAnimation( ANIMATION_TYPE toAdd ) {
		animations.push_back( toAdd );
	}

	template< int LAYER_CONSTANT >
	size_t Sprite< LAYER_CONSTANT >::ObtainAmountOfAnimations() {
		return animations.size();
	}

	template< int LAYER_CONSTANT >
	ANIMATION_TYPE Sprite< LAYER_CONSTANT >::ObtainAnimation( size_t animation ) {
		return animations[ animation ];
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetSprite( sf::Sprite sprite_ ) {
		sprite = sprite_;
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetCurrentDirection( Direction currentDirection_ ) {
		currentDirection = currentDirection_;
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetFrames( FRAMES_TYPE frames_ ) {
		frames = frames_;
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetAnimations( std::vector< ANIMATION_TYPE > animations_ ) {
		animations = animations_;
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetCurrentFrame( size_t currentFrame_ ) {
		currentFrame = currentFrame_;
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetCurrentAnimation( size_t currentAnimation_ ) {
		currentAnimation = currentAnimation_;
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetActive( bool active_ ) {
		active = active_;
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetJSONData( std::string jsonData_ ) {
		jsonData = jsonData_;
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetPlaceInSpriteSheet( sf::IntRect placeInSpriteSheet_ ) {
		placeInSpriteSheet = placeInSpriteSheet_;
	}

	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::SetAnimationActive( bool animationActive_ ) {
		animationActive = animationActive_;
	}

	template< int LAYER_CONSTANT >
	std::string Sprite< LAYER_CONSTANT >::GetSpriteName() {
		return spriteName;
	}

	template< int LAYER_CONSTANT >
	sf::Sprite Sprite< LAYER_CONSTANT >::GetSprite() {
		return sprite;
	}

	template< int LAYER_CONSTANT >
	FRAMES_TYPE Sprite< LAYER_CONSTANT >::GetFrames() {
		return frames;
	}

	template< int LAYER_CONSTANT >
	std::vector< ANIMATION_TYPE > Sprite< LAYER_CONSTANT >::GetAnimations() {
		return animations;
	}

	template< int LAYER_CONSTANT >
	Direction Sprite< LAYER_CONSTANT >::GetCurrentDirection() {
		return currentDirection;
	}

	template< int LAYER_CONSTANT >
	size_t Sprite< LAYER_CONSTANT >::GetCurrentFrame() {
		return currentFrame;
	}

	template< int LAYER_CONSTANT >
	size_t Sprite< LAYER_CONSTANT >::GetCurrentAnimation() {
		return currentAnimation;
	}

	template< int LAYER_CONSTANT >
	bool Sprite< LAYER_CONSTANT >::GetActive() {
		return active;
	}

	template< int LAYER_CONSTANT >
	std::string Sprite< LAYER_CONSTANT >::GetJSONData() {
		return jsonData;
	}

	template< int LAYER_CONSTANT >
	sf::IntRect Sprite< LAYER_CONSTANT >::GetPlaceInSpriteSheet() {
		return placeInSpriteSheet;
	}

	template< int LAYER_CONSTANT >
	bool Sprite< LAYER_CONSTANT >::GetAnimationActive() {
		return animationActive;
	}
	template< int LAYER_CONSTANT >
	float Sprite< LAYER_CONSTANT >::GetAnimationRate() {
		return animationRate;
	}


	const std::string SPRITE_FILE_NAME_ROOT_CONSTANT = "sizeAbove";

	const std::string ASSETS_FOLDER_CONSTANT = "Assets";

	template< int LAYER_CONSTANT >
	std::optional< int > Sprite< LAYER_CONSTANT >::ObtainLoadedSpriteIndex( std::string spriteName,
			std::vector< LOADED_SPRITE_DATA_TYPE >* loadedSprites )
	{
		if( loadedSprites == nullptr )
			loadedSprites = &Detail::spritesLoaded;
		const size_t LOADED_SPRITES_SIZE_CONSTANT = loadedSprites->size();
		for( size_t i = 0; i < LOADED_SPRITES_SIZE_CONSTANT; ++i ) {
			if( std::get< 0 >( loadedSprites->at( i ) ) == spriteName )
				return i;
		}
		return std::nullopt;
	}

	template< int LAYER_CONSTANT >
	sf::IntRect Sprite< LAYER_CONSTANT >::LoadSpriteImage( std::string spriteName, sf::Texture* buffer,
			sf::Image* spriteBuffer, std::vector< LOADED_SPRITE_DATA_TYPE >* loadedSprites )
	{
		if( buffer == nullptr )
		{
			if( Detail::masterTextureInitialized == false )
			{
				Detail::masterTexture = new sf::Texture;
				Detail::masterSpriteBuffer = new sf::Image;
				loadedSprites = &Detail::spritesLoaded;
				Detail::masterTextureInitialized = true;
			}
			buffer = Detail::masterTexture;
			spriteBuffer = Detail::masterSpriteBuffer;
		}
		sf::Image newImage;
		sf::IntRect spriteSourceBounds;
		newImage.loadFromFile( "Assets/" + spriteName + "/" + SPRITE_FILE_NAME_ROOT_CONSTANT + ".png" );
		//std::cout << "Assets / " + spriteName + " / " + SPRITE_FILE_NAME_ROOT_CONSTANT + ".png" << "\n";
		const size_t Y_AXIS_SIZE_CONSTANT = std::max( newImage.getSize().y, buffer->getSize().y );
		if( buffer->getSize().y > 0 )
		{
			std::cout << "A " << spriteName << "\n";
			sf::Texture intermediateTexture;
			spriteSourceBounds = sf::IntRect( buffer->getSize().x, 0, newImage.getSize().x, newImage.getSize().y );
			intermediateTexture.create(
						buffer->getSize().x + newImage.getSize().x, Y_AXIS_SIZE_CONSTANT );
			intermediateTexture.update( *buffer );
			intermediateTexture.update( newImage, buffer->getSize().x, 0 );
			*spriteBuffer = intermediateTexture.copyToImage();
			buffer->create( intermediateTexture.getSize().x, intermediateTexture.getSize().y );
			buffer->update( intermediateTexture );
		}
		else
		{
			std::cout << "B" << spriteName << "\n";
			buffer->create( newImage.getSize().x, Y_AXIS_SIZE_CONSTANT );
			buffer->update( newImage );
			*spriteBuffer = buffer->copyToImage();
			spriteSourceBounds = sf::IntRect( 0, 0, newImage.getSize().x, newImage.getSize().y );
		}
		return spriteSourceBounds;
	}

	template< int LAYER_CONSTANT >
	std::string Sprite< LAYER_CONSTANT >::LoadJSONData( std::string spriteName )
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
			std::cerr << "StrategyGoo::Sprite< LAYER_CONSTANT >::LoadJSONData( std::string ) : void::Error: failed to read file " <<
					JSON_FILE_NAME_CONSTANT << "\n";
		}
		return jsonDataStream.str();
	}

	template< int LAYER_CONSTANT >
	LOADED_SPRITE_DATA_TYPE Sprite< LAYER_CONSTANT >::LoadSprite( std::string spriteName, sf::Texture* buffer,
			sf::Image* spriteBuffer, std::vector< LOADED_SPRITE_DATA_TYPE >* loadedSprites )
	{
		const auto SPRITE_INDEX_CONSTANT = ObtainLoadedSpriteIndex( spriteName, loadedSprites );
		if( loadedSprites == nullptr )
			loadedSprites = &Detail::spritesLoaded;
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
			loadedSprites->push_back( data );
			return data;
		}
		return loadedSprites->at( SPRITE_INDEX_CONSTANT.value() );
	}
	
	template< int LAYER_CONSTANT >
	void Sprite< LAYER_CONSTANT >::InitializeSprite( std::string spriteName_ )
	{
		spriteName = spriteName_;
		auto data = LoadSprite( spriteName );
		frames = std::get< LOADED_SPRITE_DATA_TYPE_SPRITE_FRAMES_CONSTANT >( data );
		jsonData = std::get< LOADED_SPRITE_DATA_TYPE_SPRITE_JSON_DATA_CONSTANT >( data );
		placeInSpriteSheet = std::get< LOADED_SPRITE_DATA_TYPE_SPRITE_INT_RECT_CONSTANT >( data );
	}


	const std::string SPRITE_FOLDER_CONSTANT = ( "/" + ( SPRITE_FILE_NAME_ROOT_CONSTANT + "/" ) );

	const std::string SPRITE_PREFIX_FOR_DIRECTION = "_Above_";

	const size_t SPRITE_SHEET_PACKER_MAX_POWER_OF_10_CONSTANT = 2;

	template< int LAYER_CONSTANT >
	FRAMES_TYPE Sprite< LAYER_CONSTANT >::FrameDataFromJSONMetaData( std::string metaData, std::string spriteName )
	{
		FRAMES_TYPE frames;
		auto json = nlohmann::json::parse( metaData );
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
		for( size_t i = 0; i < AMOUNT_OF_DIRECTIONS_CONSTANT; ++i )
		{
			const std::string DIRECTION_CONSTANT = ( SPRITE_ROOT_NAME_CONSTANT + 
					STRING_DIRECTIONS_ABBREVIATIONS_CONSTANT[ i ] );
			if( json.contains( DIRECTION_CONSTANT + makeFrameString( 0 ) ) == true )
			{
				std::vector< sf::IntRect > frameBuffer;
				for( size_t j = 0; json.contains( 
						DIRECTION_CONSTANT + makeFrameString( j ) ) == true; ++j )
					frameBuffer.push_back( jsonDataToFrame( json[ ( DIRECTION_CONSTANT + makeFrameString( j ) ) ] ) );
				frames[ i ].insert( frames[ i ].end(), frameBuffer.begin(), frameBuffer.end() );
			}
			else if( json.contains( DIRECTION_CONSTANT ) == true )
				frames[ i ].push_back( jsonDataToFrame( json[ DIRECTION_CONSTANT ] ) );
			else
			{
				std::cerr << "StrategyGoo::Sprite< LAYER_CONSTANT >::FrameDataFromJSONMetaData( std::string, std::string ) : "
						"FRAMES_TYPE::Error: Failed to load frames for \"" <<
						STRING_DIRECTIONS_ABBREVIATIONS_CONSTANT[ i ] << "\" direction.\n";
			}
		}
		return frames;
	}

	template< int LAYER_CONSTANT >
	Sprite< LAYER_CONSTANT >::operator sf::Sprite() {
		return sprite;
	}
	
	template< int LAYER_CONSTANT >
	sf::Sprite& Sprite< LAYER_CONSTANT >::operator*() {
		return RefrenceSprite();
	}
	
	template< int LAYER_CONSTANT >
	sf::Sprite& Sprite< LAYER_CONSTANT >::RefrenceSprite() {
		return sprite;
	}
	
	template< int LAYER_CONSTANT >
	std::vector< ANIMATION_TYPE >& Sprite< LAYER_CONSTANT >::RefrenceAnimations() {
		return animations;
	}
	
	template< int LAYER_CONSTANT >
	ANIMATION_TYPE& Sprite< LAYER_CONSTANT >::RefrenceAnimation( size_t animation ) {
		return animations[ animation ];
	}

	template Sprite< -1 >;
	template Sprite< 0 >;
	template Sprite< 1 >;
}
