#include "Sprite.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <string_view>

namespace BioGooContainmentSquad
{
	namespace Detail
	{
		sf::Texture* masterTexture = nullptr;
		sf::Image* masterSpriteBuffer = nullptr;
		bool masterTextureInitialized = false;
		std::vector< LOADED_SPRITE_DATA_TYPE > spritesLoaded;
	}

	void CleanUpTextureMemory() {
		delete Detail::masterTexture;
		delete Detail::masterSpriteBuffer;
	}

	
	Sprite::Sprite( std::string spriteName_, int layer_, Direction currentDirection_ ) : layer( layer_ ), currentDirection( currentDirection_ ), currentFrame( 0 )
	{
		InitializeSprite( spriteName_ );
		ConstructDefaultAnimation();
		sprite = sf::Sprite( *Detail::masterTexture, frames[ ( unsigned short ) currentDirection ][ 0 ] );
		sprite.setTextureRect( frames[ 0 ][ 0 ] );
	}

	
	void Sprite::ConstructDefaultAnimation()
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

	
	void Sprite::Animate()
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

	
	void Sprite::Draw( sf::RenderWindow& toRenderTo )
	{
		if( active )
			toRenderTo.draw( sprite );
		Animate();
	}

	
	void Sprite::ChangeAnimation( size_t to ) {
		currentAnimation = to;
		//currentFrame = 0;
	}

	
	std::vector< sf::IntRect > Sprite::ObtainFramesForDirection( Direction spriteDirection ) {
		return frames[ ( unsigned short ) spriteDirection ];
	}

	
	std::vector< size_t > Sprite::ObtainAnimationFramesForDirection( Direction spriteDirection ) {
		return animations[ currentAnimation ][ ( unsigned short ) spriteDirection ];
	}

	
	void Sprite::SetAnimationRate( float animationRate_ ) {
		animationRate = animationRate_;
	}
		
	sf::IntRect Sprite::ObtainAnimationFrameBounds( size_t animation, Direction spriteDirection, size_t frame ) {
		/*std::cerr << "-------\nSprite Direction " << ( unsigned int ) spriteDirection << " frame size " << frames.size() << "\n";
		std::cerr << "Animation " << animation << " animations size " << animations.size() << "\n";
		std::cerr << "Sprite Direction " << ( unsigned int ) spriteDirection << " animation size " << animations[ animation ].size() << "\n";
		std::cerr << "Sprite Frame " << frame << " animation direction size " << animations[ animation ][ ( unsigned short ) spriteDirection ].size() << "+++++\n";
		*/return frames[ ( unsigned short ) spriteDirection ][
				animations[ animation ][ ( unsigned short ) spriteDirection ][ frame ] ];
	}

	
	sf::IntRect Sprite::ObtainCurrentAnimationFrameBounds( Direction spriteDirection, size_t frame ) {
		return ObtainAnimationFrameBounds( currentAnimation, spriteDirection, frame );
	}

	
	size_t Sprite::ObtainFramesForAnimation( Direction spriteDirection, size_t animation ) {
		return animations[ animation ][ ( unsigned short ) spriteDirection ].size();
	}

	
	size_t Sprite::ObtainFramesForCurrentAnimation() {
		return ObtainFramesForAnimation( currentDirection, currentAnimation );
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

	void Sprite::SetLayer( int layer_ ) {
		layer = layer_;
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

	
	void Sprite::SetAnimationActive( bool animationActive_ ) {
		animationActive = animationActive_;
	}

	int Sprite::GetLayer() {
		return layer;
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

	
	bool Sprite::GetAnimationActive() {
		return animationActive;
	}
	
	float Sprite::GetAnimationRate() {
		return animationRate;
	}

	const sf::Vector2f& Sprite::GetPosition() {
		return sprite.getPosition();
	}


	const std::string SPRITE_FILE_NAME_ROOT_CONSTANT = "sizeAbove";

	const std::string ASSETS_FOLDER_CONSTANT = "Assets";

	
	std::optional< int > Sprite::ObtainLoadedSpriteIndex( std::string spriteName,
			std::vector< LOADED_SPRITE_DATA_TYPE >* loadedSprites )
	{
		if( loadedSprites == nullptr )
			loadedSprites = &Detail::spritesLoaded;
		const size_t LOADED_SPRITES_SIZE_CONSTANT = loadedSprites->size();
		for( size_t i = 0; i < LOADED_SPRITES_SIZE_CONSTANT; ++i ) {
			if( std::get< 0 >( loadedSprites->at( i ) ) == spriteName )
				return ( int ) i;
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
		const size_t Y_AXIS_SIZE_CONSTANT = ( size_t ) std::max( newImage.getSize().y, buffer->getSize().y );
		if( buffer->getSize().y > 0 )
		{
			//std::cout << "A " << spriteName << "\n";
			sf::Texture intermediateTexture;
			spriteSourceBounds = sf::IntRect( buffer->getSize().x, 0, newImage.getSize().x, newImage.getSize().y );
			intermediateTexture.create(
						( unsigned int ) buffer->getSize().x + ( unsigned int ) newImage.getSize().x, 
						( unsigned int ) Y_AXIS_SIZE_CONSTANT );
			intermediateTexture.update( *buffer );
			intermediateTexture.update( newImage, buffer->getSize().x, 0 );
			*spriteBuffer = intermediateTexture.copyToImage();
			buffer->create( intermediateTexture.getSize().x, intermediateTexture.getSize().y );
			buffer->update( intermediateTexture );
		}
		else
		{
			//std::cout << "B" << spriteName << "\n";
			buffer->create( ( unsigned int ) newImage.getSize().x, ( unsigned int ) Y_AXIS_SIZE_CONSTANT );
			buffer->update( newImage );
			*spriteBuffer = buffer->copyToImage();
			spriteSourceBounds = sf::IntRect( 0, 0, newImage.getSize().x, newImage.getSize().y );
		}
		return spriteSourceBounds;
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
			std::cerr << "StrategyGoo::Sprite::LoadJSONData( std::string ) : void::Error: failed to read file " <<
					JSON_FILE_NAME_CONSTANT << "\n";
		}
		return jsonDataStream.str();
	}

	
	LOADED_SPRITE_DATA_TYPE Sprite::LoadSprite( std::string spriteName, sf::Texture* buffer,
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
	
	
	void Sprite::InitializeSprite( std::string spriteName_ )
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

	
	FRAMES_TYPE Sprite::FrameDataFromJSONMetaData( std::string metaData, std::string spriteName )
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
				std::cerr << "StrategyGoo::Sprite::FrameDataFromJSONMetaData( std::string, std::string ) : "
						"FRAMES_TYPE::Error: Failed to load frames for \"" <<
						STRING_DIRECTIONS_ABBREVIATIONS_CONSTANT[ i ] << "\" direction.\n";
			}
		}
		return frames;
	}

	void Sprite::SetPosition( sf::Vector2f position ) {
		sprite.setPosition( position );
	}

	void Sprite::SetPosition( float x, float y ) {
		return sprite.setPosition( x, y );
	}

	void Sprite::SetOrientation( float orientation ) {
		sprite.setRotation( orientation );
	}

	void Sprite::Move( sf::Vector2f displacement ) {
		sprite.move( displacement );
	}

	void Sprite::Move( float x, float y ) {
		sprite.move( x, y );
	}

	void Sprite::Rotate( float angle ) {
		sprite.rotate( angle );
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
