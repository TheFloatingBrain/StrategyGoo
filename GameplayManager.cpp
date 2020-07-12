#include "GameplayManager.hpp"
#include <list>
#include <iostream>
namespace StrategyGoo
{
	GameplayManager::GameplayManager( entt::registry& registry_, size_t width, size_t height ) :
			registry( registry_ ), gameBoard( registry_, width, height ), move( "Move" ),
			grenade( "Grenade" ), flameThrower( "CubeExplosion1" ), check( "Check" ), hand( "Hand" ), 
			leftArrow( "Arrow" ), rightArrow( "Arrow" ), littleTarget( "Target" ), littleMove( "Move" ), 
			throwGrenade( "Grenade" ), uiElements( { &move, &grenade, &flameThrower, &hand, &check, &leftArrow,
			&rightArrow, &littleTarget, &littleMove } ), actionBarSprites( { &grenade, &flameThrower, &move, &hand, &check } ) {
		size_t count = 0;
		for( Sprite< -1 >* element : uiElements ) {
			element->RefrenceSprite().setPosition( 64 + ( 64 * count++ ), 128 );
		}
		InitilizeUIComponents();
	}

	void GameplayManager::InitilizeUIComponents()
	{
		actionBar.left = 320;
		actionBar.top = 704;
		actionBar.width = 64 * 7;
		actionBar.height = 64;
		leftArrow.SetCurrentDirection( Direction::WEST );
		( *leftArrow ).setPosition( actionBar.left, actionBar.top );
		rightArrow.SetCurrentDirection( Direction::EAST );
		( *rightArrow ).setPosition( actionBar.left + ( 64 * 6 ), actionBar.top );
		( *grenade ).scale( 1.0f, .8f );
		( *grenade ).setPosition( actionBar.left + 64, actionBar.top );
		( *flameThrower ).setPosition( actionBar.left + ( 64 * 2 ), actionBar.top );
		( *move ).setPosition( actionBar.left + ( 64 * 3 ), actionBar.top );
		( *hand ).setPosition( actionBar.left + ( 64 * 4 ), actionBar.top );
		hand.SetCurrentDirection( Direction::NORTH_EAST );
		( *check ).setPosition( actionBar.left + ( 64 * 5 ), actionBar.top );
		check.SetCurrentDirection( Direction::EAST );
		( *littleMove ).scale( .5f, .5f );
		( *littleTarget ).scale( .5f, .5f );
	}

	template< typename ENTITY_TYPE >
	ENTITY_TYPE& GameplayManager::CreateEntity( BoardPosition startingPosition )
	{
		ENTITY_TYPE* newEntity = new ENTITY_TYPE( registry, startingPosition, &gameBoard,
				gameBoard.GetTileWidthConstant(), gameBoard.GetTileHeightConstant() );
		entities.push_back( newEntity );
		return *newEntity;
	}
	void GameplayManager::Update()
	{
	}
	void GameplayManager::Render( sf::RenderWindow& window )
	{
		window.clear();
		UpdatePlayer< MoveOrder >();
		registry.view< Sprite< 1 > >().each( [&]( auto& tile, auto& sprite ) {
			sprite.Draw( window );
			}
		);
		registry.view< Sprite< 0 > >().each( [ & ]( auto& tile, auto& sprite ) {
				sprite.Draw( window );
			}
		);
		DrawGUI( window );
		///window.draw( sf::Sprite( *Detail::masterTexture ) );
		PlayerGiveOrdersStage( window );
		window.display();
	}

	template< typename ORDER_TYPE >
	bool GameplayManager::UpdatePlayer()
	{
		std::list< entt::entity > doneMoving;
		bool allDone = true;
		registry.view< Squaddie::SquaddieRefrence, ORDER_TYPE >().each(
			[ & ]( Squaddie::SquaddieRefrence& squaddie, MoveOrder& order ) {
				entt::entity id = squaddie.get().GetID();
				if( allDone = ( allDone && order.Tick( squaddie.get(), registry ) ) )
					doneMoving.push_back( id );
			} );
		for( auto currentEntity : doneMoving )
			registry.remove< ORDER_TYPE >( currentEntity );
		return allDone;
	}
	
	void GameplayManager::DrawGUI( sf::RenderWindow& window )
	{
		sf::RectangleShape actionPanelRender;
		actionPanelRender.setPosition( sf::Vector2f( ( float ) actionBar.left, ( float ) actionBar.top ) );
		actionPanelRender.setSize( sf::Vector2f( ( float ) actionBar.width, ( float ) actionBar.height ) );
		actionPanelRender.setFillColor( sf::Color( 97, 90, 90, 255 ) );
		actionPanelRender.setOutlineColor( sf::Color( 14, 32, 232, 255 ) );
		actionPanelRender.setOutlineThickness( 2.0f );
		for( size_t i = 0; i < actionBarSprites.size(); ++i )
		{
			if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
			{
				sf::IntRect actionBoundingBox;
				auto textureRectangle = ( actionBarSprites[ 0 ]->RefrenceSprite().getTextureRect() );
				actionBoundingBox.width = textureRectangle.width;
				actionBoundingBox.height = textureRectangle.height;
				actionBoundingBox.left = actionBarSprites[ 0 ]->RefrenceSprite().getPosition().x;
				actionBoundingBox.top = actionBarSprites[ 0 ]->RefrenceSprite().getPosition().y;
				if( actionBoundingBox.contains( sf::Mouse::getPosition( window ) ) == true ) {
					currentAction = i;
					break;
				}
			}
		}
		window.draw( actionPanelRender );
		leftArrow.Draw( window );
		rightArrow.Draw( window );
		for( Sprite< -1 > * uiElement : actionBarSprites )
			uiElement->Draw( window );

	}

	void GameplayManager::PlayerGiveOrdersStage( sf::RenderWindow& window )
	{
		auto selectionData = Squaddie::SelectSquaddie( registry, window );
		if( selectionData.first == true )
			idOfSelectedSquaddie = selectionData.second;
		if( idOfSelectedSquaddie.has_value() == true )
		{
			if( sf::Mouse::isButtonPressed( sf::Mouse::Button::Right ) == true )
			{
				if( actionBar.contains( sf::Mouse::getPosition( window ) ) == false )
				{
					registry.remove_if_exists< MoveOrder >( idOfSelectedSquaddie.value() );
					registry.emplace< MoveOrder >( idOfSelectedSquaddie.value(),
							registry.get< BoardPosition >( idOfSelectedSquaddie.value() ),
							gameBoard.ToBoardCoordinates( sf::Mouse::getPosition( window ) ) );
				}
			}
		}
	}

	entt::registry& GameplayManager::RefrenceRegistry() {
		return registry;
	}

	GameBoard& GameplayManager::RefrenceGameBoard() {
		return gameBoard;
	}

	template Squaddie& GameplayManager::CreateEntity< Squaddie >( BoardPosition );
	template Goo& GameplayManager::CreateEntity< Goo >( BoardPosition );

}
