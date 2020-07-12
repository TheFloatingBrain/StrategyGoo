#include "GameplayManager.hpp"
#include <list>
#include <iostream>
namespace StrategyGoo
{
	GameplayManager::GameplayManager( entt::registry& registry_, size_t width, size_t height ) :
			registry( registry_ ), gameBoard( registry_, width, height ), move( "Move" ),
			grenade( "Grenade" ), target( "Target" ), leftArrow( "Arrow" ), rightArrow( "Arrow" ),
			littleTarget( "Target" ), littleMove( "Move" ), uiElements( { &move, &grenade, &target, &leftArrow,
			&rightArrow, &littleTarget, &littleMove }  ) {
		int count = 0;
		for( Sprite< -1 >* element : uiElements ) {
			element->RefrenceSprite().setPosition( 64 + ( 64 * count ), 128 );
			++count;
//			element.SetAnimationActive( false );
		}
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
		window.draw( sf::Sprite( *Detail::masterTexture ) );
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
		for( Sprite< -1 >* uiElement : uiElements )
			window.draw( uiElement->RefrenceSprite() );
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
				registry.remove_if_exists< MoveOrder >( idOfSelectedSquaddie.value() );
				registry.emplace< MoveOrder >( idOfSelectedSquaddie.value(),
						registry.get< BoardPosition >( idOfSelectedSquaddie.value() ), 
						gameBoard.ToBoardCoordinates( sf::Mouse::getPosition( window ) ) );
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
