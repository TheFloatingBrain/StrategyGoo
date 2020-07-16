#include "GameplayManager.hpp"
#include <list>
#include <iostream>
#include <typeindex>
namespace StrategyGoo
{
	GameplayManager::GameplayManager( entt::registry& registry_, size_t width, size_t height ) :
			registry( registry_ ), gameBoard( registry_, width, height ), move( "Move" ),
			grenade( "Grenade" ), flameThrower( "CubeExplosion1" ), check( "Check" ), hand( "Hand" ),
			leftArrow( "Arrow" ), rightArrow( "Arrow" ), littleTarget( "Target" ), littleMove( "Move" ),
			throwGrenade( "Grenade" ), defaultCursor( "Cursor" ), cursorSprite( "Cursor" ), selectionSquare( "SelectionSquare" ),
			uiElements( { &move, &grenade, &flameThrower, &hand, &check, &leftArrow,
			&rightArrow, &littleTarget, &littleMove } ), actionBarSprites( { &grenade, &flameThrower,
			&move, &hand, &check, &defaultCursor } ), winSprite( "YouWin" ), loseSprite( "YouLose" ), pressAnyKeySprite( "PressAnyKey" )
	{
		size_t count = 0;
		for( Sprite< -1 >* element : uiElements ) {
			element->RefrenceSprite().setPosition( ( float ) ( 64 + ( 64 * count++ ) ), 128.f );
		}
		InitilizeUIComponents();
		InitializeLevel();
	}

	void GameplayManager::InitializeLevel()
	{
		CreateEntity< Squaddie >( BoardPosition( 2, 4 ) ).RefrenceSprite().SetCurrentDirection( Direction::SOUTH );
		CreateEntity< Squaddie >( BoardPosition( 2, 5 ) ).RefrenceSprite().SetCurrentDirection( Direction::SOUTH );
		CreateEntity< Squaddie >( BoardPosition( 2, 6 ) ).RefrenceSprite().SetCurrentDirection( Direction::SOUTH );
		auto lastPosition = BoardPosition( 8, 5 );
		const BoardPosition POSSIBLE_OFFSET_CONSTANT[] = {
			BoardPosition( 1, 0 ), BoardPosition( 0, 1 ),
			BoardPosition( -1, 0 ), BoardPosition( 0, -1 ),
			BoardPosition( -1, -1 ), BoardPosition( 1, 1 )
		};
		Goo& goo = CreateEntity< Goo >( lastPosition );
		std::vector< BoardPosition > previousGooPositions;
		sf::IntRect boardConstraints{ 0, 0, 
				( int ) gameBoard.GetWidth(), ( int ) gameBoard.GetHeight() };
		for( size_t i = 0; i < 8; ++i )
		{
			BoardPosition currentPosition = lastPosition +
					POSSIBLE_OFFSET_CONSTANT[ RandomRange( 0, 5 ) ];
			if( previousGooPositions.size() > 0 )
			{
				for( size_t j = 0;
						std::find( previousGooPositions.begin(),
						previousGooPositions.end(), currentPosition ) !=
						previousGooPositions.end() &&
						boardConstraints.contains( currentPosition );
						currentPosition = ( lastPosition + POSSIBLE_OFFSET_CONSTANT[ j++ ] ) );
			}
			else if( boardConstraints.contains( currentPosition ) == false )
					currentPosition = BoardPosition( 9, 5 );
			previousGooPositions.push_back( currentPosition );
			goo.AddGoo( currentPosition );
			lastPosition = currentPosition;
		}
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
		littleMove.SetActive( false );
		littleTarget.SetActive( false );
		defaultCursor.SetCurrentDirection( Direction::SOUTH );
		defaultCursor.SetActive( false );
		cursorSprite.SetCurrentDirection( defaultCursor.GetCurrentDirection() );
		selectionSquare.SetActive( false );
		( *winSprite ).setPosition( ( float ) actionBar.left, 128.0f );
		( *winSprite ).scale( 6.f, 4.f );
		winSprite.SetCurrentDirection( Direction::EAST );
		( *loseSprite ).setPosition( ( float ) actionBar.left, 128.0f );
		( *loseSprite ).scale( 6.f, 4.f );
		loseSprite.SetCurrentDirection( Direction::EAST );
		( *pressAnyKeySprite ).setPosition( ( float ) actionBar.left, 384 );
		( *pressAnyKeySprite ).scale( 8.f, 4.f );
		pressAnyKeySprite.SetCurrentDirection( Direction::EAST );
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
		switch( gameState )
		{
			case StagesOfPlay::PLAYER_GIVE_ORDERS_STAGE : {
				PlayerGiveOrdersStage( window );
				break;
			}
			case StagesOfPlay::PLAYER_EXECUTE_ORDERS_STAGE : {
				ExecuteAllPlayerOrders();
				break;
			}
			case StagesOfPlay::SLIME_MOVE_STAGE : {
				GooMove( window );
				break;
			}
			case StagesOfPlay::PLAYER_DAMAGE_STAGE : {
				PlayerDamageStage();
				break;
			}
			case StagesOfPlay::LOOSE : {
				gameState = StagesOfPlay::LOOSE;
				break;
			}
			case StagesOfPlay::WIN: {
				gameState = StagesOfPlay::WIN;
				break;
			}
			default : {
				break;
			}
		}
	}

	void GameplayManager::RemoveSquaddie( Squaddie::SquaddieRefrence toRemove )
	{
		if( idOfSelectedSquaddie.has_value() ) {
			if( idOfSelectedSquaddie.value() == toRemove.get().GetID() )
				idOfSelectedSquaddie = std::nullopt;
		}
		RemoveEntityFromTile< Squaddie::SquaddieRefrence >( registry,
			toRemove.get().RefrenceBoardPosition(), toRemove.get().GetBoard() );
		for( size_t i = 0; i < entities.size(); ++i )
		{
			if( entities[ i ]->GetID() == toRemove.get().GetID() )
			{
				delete entities[ i ];
				entities.erase( entities.begin() + i );
				break;
			}
		}
		registry.remove_all( toRemove.get().GetID() );
		registry.destroy( toRemove.get().GetID() );
	}

	void GameplayManager::Render( sf::RenderWindow& window )
	{
		window.clear();
		registry.view< Sprite< 1 > >().each( [&]( auto& sprite ) {
			sprite.Draw( window );
			}
		);
		registry.view< Sprite< 0 >, Goo::GooComponentRefrence >().each( [ & ]( 
			Sprite< 0 >& sprite, Goo::GooComponentRefrence& goo ) {
				sprite.Draw( window );
			}
		);
		if( gameState == StagesOfPlay::PLAYER_GIVE_ORDERS_STAGE )
		{
			selectionSquare.Draw( window );
			littleMove.Draw( window );
			littleTarget.Draw( window );
		}
		registry.view< Sprite< 0 > >( entt::exclude< Goo::GooComponentRefrence > ).each( [&](
			Sprite< 0 >& sprite ) {
				sprite.Draw( window );
			}
		);
		registry.view< FlamethrowerOrder::FlameSpriteType >().each( [&]( FlamethrowerOrder::FlameSpriteType& flamethrowerSprites ) {
			for( auto& sprite : flamethrowerSprites )
				sprite.Draw( window );
			} 
		);
		DrawGUI( window );
		registry.view< Sprite< 2 > >().each( [&](
			Sprite< 2 >& sprite ) {
				sprite.Draw( window );
			}
		);
		if( gameState == StagesOfPlay::WIN ) {
			winSprite.Draw( window );
			pressAnyKeySprite.Draw( window );
		}
		else if( gameState == StagesOfPlay::LOOSE ) {
			loseSprite.Draw( window );
			pressAnyKeySprite.Draw( window );
		}
		window.display();
	}
	void GameplayManager::PlayerDamageStage()
	{
		std::list< Squaddie::SquaddieRefrence > toDelete;
		registry.view< Goo::GooComponentRefrence, Squaddie::SquaddieRefrence >().each(
				[&]( Goo::GooComponentRefrence& tile, Squaddie::SquaddieRefrence& squaddie ) {
					toDelete.push_back( squaddie );
				} );
		for( auto squaddie : toDelete )
			RemoveSquaddie( squaddie );
		gameState = ( ( registry.size< Squaddie::SquaddieRefrence >() > 0 ) ?
				StagesOfPlay::PLAYER_GIVE_ORDERS_STAGE : StagesOfPlay::LOOSE );
	}
	void GameplayManager::GooMove( sf::RenderWindow& window )
	{
		/*for( auto ent : debug ) {
			registry.remove_all( ent );
			registry.destroy( ent );
		}
		debug.clear();*/

		auto view = registry.view< Squaddie::SquaddieRefrence >();
		Goo* goo = nullptr;
		if( view.size() > 0 )
		{
			for( auto* currentEntity : entities )
			{
				if( goo = dynamic_cast< Goo* >( currentEntity ) )
				{
					size_t squaddieToMoveToo = ( size_t ) RandomRange( 0, ( int ) view.size() - 1 );
					auto squaddie = registry.get< Squaddie::SquaddieRefrence >( view[ squaddieToMoveToo ] );
					if( RandomRange( 0, 10 ) >= 8 )
					{
						goo->MoveToward( squaddie.get().RefrenceBoardPosition() );
						goo->MoveToward( squaddie.get().RefrenceBoardPosition() );
						goo->MoveToward( squaddie.get().RefrenceBoardPosition() );
					}
					else
					{
						goo->MoveToward( squaddie.get().RefrenceBoardPosition(), 1, true );
						goo->MoveToward( squaddie.get().RefrenceBoardPosition(), 1, true );
						goo->MoveToward( squaddie.get().RefrenceBoardPosition(), 1, true );
					}
					/*for( auto& gooComp : goo->GetGoo() ) {
						std::cout << "G: " << gooComp->RefrenceBoardPosition().x << ", " << gooComp->RefrenceBoardPosition().y << "\n";
						auto ref = registry.create();
						registry.emplace< Sprite< 2 > >( ref, "Hand" );
						registry.get< Sprite< 2 > >( ref ).RefrenceSprite().setPosition( gameBoard.ToWorldCoordinates( gooComp->RefrenceBoardPosition() ) );
						debug.push_back( ref );
					}*/
				}
			}
		}
		/*std::cout << "Amount of entities " << entities.size() << "\n";
		std::cout << "Goo detected " << ( goo == nullptr ) << "\n";*/
		gameState = ( ( registry.size< Goo::GooComponentRefrence >() > 0 ) ?
				StagesOfPlay::PLAYER_DAMAGE_STAGE : StagesOfPlay::WIN );
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

	void GameplayManager::ExecuteAllPlayerOrders()
	{
		orderCoordinates.clear();
		selectionSquare.SetActive( false );
		bool allOrderTypesComplete = true;
		allOrderTypesComplete = (
			allOrderTypesComplete && UpdatePlayer< MoveOrder >() &&
			UpdatePlayer< ShootGrenadeOrder >() && 
			UpdatePlayer< FlamethrowerOrder >() );
		if( allOrderTypesComplete )
			gameState = StagesOfPlay::SLIME_MOVE_STAGE;
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
		bool renderOrders = false;
		if( idOfSelectedSquaddie.has_value() == true && onActionBar == false )
		{
			std::optional< std::reference_wrapper< std::tuple< entt::entity, sf::Vector2i, bool > > > orderData;
			for( auto& coordinate : orderCoordinates )
			{
				if( std::get< 0 >( coordinate ) == idOfSelectedSquaddie.value() ) {
					orderData = coordinate;
					break;
				}
			}
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
					bool orderGiven = false;
					bool targetOrder = false;
					registry.remove_if_exists< MoveOrder >( idOfSelectedSquaddie.value() );
					registry.remove_if_exists< ShootGrenadeOrder >( idOfSelectedSquaddie.value() );
					registry.remove_if_exists< FlamethrowerOrder >( idOfSelectedSquaddie.value() );
					const auto PLAYER_COORDINATES_CONSTANT = registry.get< BoardPosition >( idOfSelectedSquaddie.value() );
					auto boardCoordinates = gameBoard.ToBoardCoordinates( mousePosition );
					PrintVect( boardCoordinates );
					const auto ORDER_DISPLACEMENT_CONSTANT = ConditionalMagnitude< int >( boardCoordinates - PLAYER_COORDINATES_CONSTANT );
					if( currentAction == PlayerAction::MOVE &&
						ORDER_DISPLACEMENT_CONSTANT <= PlayerOrderMaxDistance< MoveOrder >::MAX_DISTANCE_CONSTANT )
					{
						registry.emplace< MoveOrder >( idOfSelectedSquaddie.value(),
							registry.get< BoardPosition >( idOfSelectedSquaddie.value() ),
							boardCoordinates );
						orderGiven = true;
					}
					if( currentAction == PlayerAction::GRENADE &&
						ORDER_DISPLACEMENT_CONSTANT <= PlayerOrderMaxDistance< ShootGrenadeOrder >::MAX_DISTANCE_CONSTANT )
					{
						registry.emplace< ShootGrenadeOrder >( idOfSelectedSquaddie.value(),
							registry.get< BoardPosition >( idOfSelectedSquaddie.value() ),
							boardCoordinates );
						targetOrder = true;
						orderGiven = true;
					}
					if( currentAction == PlayerAction::FLAME_THROWER &&
						ORDER_DISPLACEMENT_CONSTANT <= PlayerOrderMaxDistance< FlamethrowerOrder >::MAX_DISTANCE_CONSTANT )
					{
						auto& squaddie = registry.get< Squaddie::SquaddieRefrence >( idOfSelectedSquaddie.value() );
						registry.emplace< FlamethrowerOrder >( idOfSelectedSquaddie.value(), squaddie, boardCoordinates );
						targetOrder = true;
						orderGiven = true;
					}
					if( orderData.has_value() == false && orderGiven )
					{
						orderCoordinates.push_back( std::tuple< entt::entity, sf::Vector2i, bool >(
							idOfSelectedSquaddie.value(), boardCoordinates, targetOrder ) );
						orderData = *( --orderCoordinates.end() );
					}
					else if( orderGiven ) {
						std::get< 1 >( orderData.value().get() ) = boardCoordinates;
						std::get< 2 >( orderData.value().get() ) = targetOrder;
					}
				}
			}
			if( orderData.has_value() == true )
			{
				renderOrders = true;
				littleMove.SetActive( !std::get< 2 >( orderData.value().get() ) );
				littleTarget.SetActive( std::get< 2 >( orderData.value().get() ) );
				( *littleMove ).setPosition( gameBoard.ToWorldCoordinates(
						std::get< 1 >( orderData.value().get() ) ) );
				( *littleTarget ).setPosition( gameBoard.ToWorldCoordinates(
						std::get< 1 >( orderData.value().get() ) ) );
			}
		}
		if( renderOrders == false ) {
			littleMove.SetActive( false );
			littleTarget.SetActive( false );
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
