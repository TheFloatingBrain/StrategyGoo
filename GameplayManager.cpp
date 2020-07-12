#include "GameplayManager.hpp"
#include <list>
#include <iostream>
namespace StrategyGoo
{
	GameplayManager::GameplayManager( entt::registry& registry_, size_t width, size_t height ) :
			registry( registry_ ), gameBoard( registry_, width, height ), move( "Move" ),
			grenade( "Grenade" ), flameThrower( "CubeExplosion1" ), check( "Check" ), hand( "Hand" ), 
			leftArrow( "Arrow" ), rightArrow( "Arrow" ), littleTarget( "Target" ), littleMove( "Move" ), 
			throwGrenade( "Grenade" ), defaultCursor( "Cursor" ), cursorSprite( "Cursor" ), selectionSquare( "SelectionSquare" ), 
			uiElements( { &move, &grenade, &flameThrower, &hand, &check, &leftArrow,
			&rightArrow, &littleTarget, &littleMove } ), actionBarSprites( { &grenade, &flameThrower, 
			&move, &hand, &check, &defaultCursor } ) {
		size_t count = 0;
		for( Sprite< -1 >* element : uiElements ) {
			element->RefrenceSprite().setPosition( ( float ) ( 64 + ( 64 * count++ ) ), 128.f );
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
		( *leftArrow ).setPosition( ( float ) actionBar.left, ( float ) actionBar.top );
		rightArrow.SetCurrentDirection( Direction::EAST );
		( *rightArrow ).setPosition( ( float ) actionBar.left + ( 64.f * 6.f ), ( float ) actionBar.top );
		( *grenade ).scale( 1.0f, .8f );
		( *grenade ).setPosition( ( float ) actionBar.left + 64.f, ( float ) actionBar.top );
		( *flameThrower ).setPosition( ( float ) actionBar.left + ( 64.f * 2.f ), ( float ) actionBar.top );
		( *move ).setPosition( ( float ) actionBar.left + ( 64.f * 3.f ), ( float ) actionBar.top );
		( *hand ).setPosition( ( float ) actionBar.left + ( 64.f * 4.f ), ( float ) actionBar.top );
		hand.SetCurrentDirection( Direction::NORTH_EAST );
		( *check ).setPosition( ( float ) actionBar.left + ( 64.f * 5.f ), ( float ) actionBar.top );
		check.SetCurrentDirection( Direction::EAST );
		( *littleMove ).scale( .5f, .5f );
		( *littleTarget ).scale( .5f, .5f );
		defaultCursor.SetCurrentDirection( Direction::SOUTH );
		defaultCursor.SetActive( false );
		cursorSprite.SetCurrentDirection( defaultCursor.GetCurrentDirection() );
		selectionSquare.SetActive( false );
	}

	template< typename ENTITY_TYPE >
	ENTITY_TYPE& GameplayManager::CreateEntity( BoardPosition startingPosition )
	{
		ENTITY_TYPE* newEntity = new ENTITY_TYPE( registry, startingPosition, &gameBoard,
				gameBoard.GetTileWidthConstant(), gameBoard.GetTileHeightConstant() );
		entities.push_back( newEntity );
		return *newEntity;
	}

	GameplayManager::StagesOfPlay GameplayManager::GetGameState() {
		return gameState;
	}

	void GameplayManager::Update( sf::RenderWindow& window )
	{
		//std::cout << "Game state: " << ( int ) gameState << "\n";
		switch( gameState )
		{
			case StagesOfPlay::PLAYER_GIVE_ORDERS_STAGE : {
				PlayerGiveOrdersStage( window );
				break;
			}
			case StagesOfPlay::PLAYER_EXECUTE_ORDERS_STAGE : 
			{
				selectionSquare.SetActive( false );
				bool allOrderTypesComplete = true;
				allOrderTypesComplete = ( 
						allOrderTypesComplete && UpdatePlayer< MoveOrder >() && 
						UpdatePlayer< ShootGrenadeOrder >() );
				if( allOrderTypesComplete )
					gameState = StagesOfPlay::SLIME_MOVE_STAGE;
				//if( allOrderTypesComplete )
				//	std::cout << "DONE\n";
				break;
			}
			case StagesOfPlay::SLIME_MOVE_STAGE : {
				SlimeMove( window );
				break;
			}
			case StagesOfPlay::PLAYER_DAMAGE_STAGE : {
				gameState = StagesOfPlay::PLAYER_GIVE_ORDERS_STAGE;
				break;
			}
			default : {
				break;
			}
		}
	}
	void GameplayManager::Render( sf::RenderWindow& window )
	{
		window.clear();
		registry.view< Sprite< 1 > >().each( [&]( auto& tile, auto& sprite ) {
			sprite.Draw( window );
			}
		);
		selectionSquare.Draw( window );
		registry.view< Sprite< 0 > >().each( [ & ]( auto& tile, auto& sprite ) {
				sprite.Draw( window );
			}
		);
		DrawGUI( window );
		window.display();
	}

	void GameplayManager::SlimeMove( sf::RenderWindow& window )
	{
		auto view = registry.view< Squaddie::SquaddieRefrence >();
		Goo* goo = nullptr;
		for( auto* currentEntity : entities )
		{
			if( goo = dynamic_cast< Goo* >( currentEntity ) )
			{
				size_t squaddieToMoveToo = ( size_t ) RandomRange( 0, view.size() - 1 );
				auto squaddie = registry.get< Squaddie::SquaddieRefrence >( view[ squaddieToMoveToo ] );
				if( RandomRange( 0, 10 ) >= 5 )
					goo->MoveToward( squaddie.get().RefrenceBoardPosition() );
				else
					goo->MoveToward( squaddie.get().RefrenceBoardPosition(), 1, true );
			}
		}
		gameState = StagesOfPlay::PLAYER_DAMAGE_STAGE;
	}

	template< typename ORDER_TYPE >
	bool GameplayManager::UpdatePlayer()
	{
		std::list< entt::entity > doneMoving;
		bool allDone = true;
		registry.view< Squaddie::SquaddieRefrence, ORDER_TYPE >().each(
			[ & ]( Squaddie::SquaddieRefrence& squaddie, ORDER_TYPE& order ) {
				entt::entity id = squaddie.get().GetID();
				bool thisOrder = order.Tick( squaddie.get(), registry );
				if( thisOrder )
					doneMoving.push_back( id );
				allDone = thisOrder && allDone;
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
		if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
		{
			currentAction = PlayerAction::NONE;
			//-1 because the last sprite is the default cursor.//
			for( size_t i = 0; i < actionBarSprites.size() - 1; ++i )
			{
				sf::Sprite& actionSprite = actionBarSprites[ i ]->RefrenceSprite();
				sf::IntRect actionBoundingBox{ ( int ) actionSprite.getPosition().x,
						( int ) actionSprite.getPosition().y, 64, 64 };
				if( actionBoundingBox.contains( sf::Mouse::getPosition( window ) ) == true ) {
					currentAction = ( PlayerAction ) i;
					break;
				}
			}
			if( currentAction == PlayerAction::END_TURN ) {
				gameState = StagesOfPlay::PLAYER_EXECUTE_ORDERS_STAGE;
				currentAction = PlayerAction::NONE;
			}
			auto* toBeCursor = actionBarSprites[ ( size_t ) currentAction ];
			cursorSprite = Sprite< -1 >( toBeCursor->GetSpriteName() );
			cursorSprite.SetCurrentDirection( toBeCursor->GetCurrentDirection() );
		}
		cursorSprite.RefrenceSprite().setPosition( ConvertVector< float, int >( sf::Mouse::getPosition( window ) ) );
		window.draw( actionPanelRender );
		leftArrow.Draw( window );
		rightArrow.Draw( window );
		for( Sprite< -1 > * uiElement : actionBarSprites )
			uiElement->Draw( window );
		cursorSprite.Draw( window );
	}

	void GameplayManager::PlayerGiveOrdersStage( sf::RenderWindow& window )
	{
		auto mousePosition = sf::Mouse::getPosition( window );
		bool onActionBar = actionBar.contains( mousePosition );
		if( onActionBar == false )
		{
			auto selectionData = Squaddie::SelectSquaddie( registry, window );
			if( selectionData.first == true )
				idOfSelectedSquaddie = selectionData.second;
			else
				selectionSquare.SetActive( false );
		}

		if( idOfSelectedSquaddie.has_value() == true && onActionBar == false )
		{
			selectionSquare.SetActive( true );
			selectionSquare.RefrenceSprite().setPosition(
					registry.get< Sprite< 0 > >( idOfSelectedSquaddie.value() ).RefrenceSprite().getPosition() );
			if( sf::Mouse::isButtonPressed( sf::Mouse::Button::Right ) == true )
			{
				
				if( RectangleFromVectors< float >( registry.get< Sprite< 0 > >( 
							idOfSelectedSquaddie.value() ).RefrenceSprite().getPosition(), 
							sf::Vector2f( 64.f, 64.f ) ).contains( 
							ConvertVector< float, int >( mousePosition ) ) == false )
				{
					registry.remove_if_exists< MoveOrder >( idOfSelectedSquaddie.value() );
					registry.remove_if_exists< ShootGrenadeOrder >( idOfSelectedSquaddie.value() );
					if( currentAction == PlayerAction::MOVE )
					{
						registry.emplace< MoveOrder >( idOfSelectedSquaddie.value(),
								registry.get< BoardPosition >( idOfSelectedSquaddie.value() ),
								gameBoard.ToBoardCoordinates( sf::Mouse::getPosition( window ) ) );
					}
					if( currentAction == PlayerAction::GRENADE )
					{
						registry.emplace< ShootGrenadeOrder >( idOfSelectedSquaddie.value(),
								registry.get< BoardPosition >( idOfSelectedSquaddie.value() ),
								gameBoard.ToBoardCoordinates( sf::Mouse::getPosition( window ) ) );
					}
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
