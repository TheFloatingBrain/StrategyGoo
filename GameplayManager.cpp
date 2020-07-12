#include "GameplayManager.hpp"
#include <list>

namespace StrategyGoo
{
	GameplayManager::GameplayManager( entt::registry& registry_ ) : 
			registry( registry_ ), gameBoard( registry_, 64, 64 ) {}

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
		PlayerGiveOrdersStage( window );
		UpdatePlayer< MoveOrder >();
		window.clear();
		registry.view< Sprite< 1 > >().each( [&]( auto& tile, auto& sprite ) {
			sprite.Draw( window );
			}
		);
		registry.view< Sprite< 0 > >().each( [ & ]( auto& tile, auto& sprite ) {
				sprite.Draw( window );
			}
		);
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
