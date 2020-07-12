#include "GameplayManager.hpp"
#include <list>
	#include <iostream>
namespace StrategyGoo
{
	bool ComparePosition( sf::Vector2f first, sf::Vector2f second ) {
		return ( abs( first.x - second.x ) < 1 ) &&
				( abs( first.y - second.y ) < 1 );
	}

	size_t ClosestFacing( sf::Vector2i vector )
	{
		sf::Vector2i unitVector = ToUnitVector< int >{ vector };
		for( size_t i = 0; i < AMOUNT_OF_DIRECTIONS_CONSTANT; ++i ) {
			if( DIRECTION_VECTOR_FACINGS_CONSTANT[ i ] == unitVector )
				return i;
		}
		return 0;
	}

	Squaddie::Squaddie( entt::registry& registry_, BoardPosition start, GameBoard* board_, size_t tileWidth, size_t tileHeight ) :
		registry( registry_ ), board( board_ ), ENTITY_TILE_WIDTH_CONSTANT( tileWidth ), ENTITY_TILE_HEIGHT_CONSTANT( tileHeight ) 
	{
		id = registry.create();
		registry.emplace< BoardPosition >( id, start.x, start.y );
		registry.emplace< Sprite< 0 > >( id, "Squaddie" );
		RefrenceSprite().RefrenceSprite().setPosition( ToWorldPosition() );
		registry.emplace< SquaddieRefrence >( id, *this );
		registry.emplace< Updator::UpdatorRefrence >( id, *this );
	}

	sf::Vector2f Squaddie::ToWorldPosition() {
		auto position = RefrenceBoardPosition();
		return sf::Vector2f( ( float ) position.x * ENTITY_TILE_WIDTH_CONSTANT, ( float ) position.y * ENTITY_TILE_WIDTH_CONSTANT );
	}

	//Anything that needs to be regularly updated.//
	void Squaddie::Update() {}
	//For animations between orders.//
	void Squaddie::TickOrder() {}

	BoardPosition& Squaddie::RefrenceBoardPosition() {
		return registry.get< BoardPosition >( id );
	}
	Sprite< 0 >& Squaddie::RefrenceSprite() {
		return registry.get< Sprite< 0 > >( id );
	}

	GameBoard* Squaddie::GetBoard() {
		return board;
	}
	entt::entity Squaddie::GetID() {
		return id;
	}


	void PrintRect( sf::FloatRect rect )
	{
		std::cout << rect.left << ", " << rect.top <<
			", " << rect.width << ", " << rect.height << "\n";
	}
	void PrintRect( sf::IntRect rect )
	{
		std::cout << rect.left << ", " << rect.top <<
			", " << rect.width << ", " << rect.height << "\n";
	}

	void PrintVect( sf::Vector2f v )
	{
		std::cout << v.x << ", " << v.y << "\n";
	}
	void PrintVect( sf::Vector2i v )
	{
		std::cout << v.x << ", " << v.y << "\n";
	}

	bool Squaddie::CheckSelect( entt::registry& registry, sf::RenderWindow& window )
	{
		sf::IntRect box = RefrenceSprite().GetSprite().getTextureRect();
		auto worldPosition = ToWorldPosition();
		sf::IntRect checkMouse{ ( int ) worldPosition.x,
			( int ) worldPosition.y, box.width, box.height };
		return checkMouse.contains( ( sf::Mouse::getPosition( window ) + 
				ConvertVector< int, float >( RectanglePosition( window.getView().getViewport() ) ) ) );
	}

	std::pair< bool, std::optional< entt::entity > > Squaddie::SelectSquaddie( entt::registry& registry, sf::RenderWindow& window )
	{
		if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
		{
			sf::View& camera = ( sf::View& ) window.getView();
			bool didSelect = false;
			entt::entity selectedID;
			registry.view< SquaddieRefrence >().each( [ & ]( SquaddieRefrence& squaddie ) {
					if( squaddie.get().CheckSelect( registry, window ) == true && didSelect == false ) {
						didSelect = true;
						selectedID = squaddie.get().id;
					}
				} );
			return std::pair< bool, std::optional< entt::entity > >( true,
					( didSelect ? std::optional< entt::entity >( selectedID ) : std::nullopt ) );
		}
		return std::pair< bool, std::optional< entt::entity > >( false, std::nullopt );
	}

	bool Squaddie::AddOrders( entt::registry& registry, entt::entity& id, sf::View& camera ) {
		return false;
	}
	bool AddOrders( entt::registry& registry, entt::entity& id, sf::View& camera ) {
		return false;
	}
	bool Squaddie::ExecuteOrders( entt::registry& registry ) {
		return false;
	}

	MoveOrder::MoveOrder( BoardPosition from_, BoardPosition to_ ) : 
			from( from_ ), to( to_ ) {}

	bool MoveOrder::Execute( Squaddie& squaddie )
	{
		auto position = squaddie.RefrenceBoardPosition();
		const sf::Vector2i MOVEMENT_CONSTANT = ToUnitVector< int >( to - position );
		position += MOVEMENT_CONSTANT;
		return position == to;
	}

	bool MoveOrder::Tick( Squaddie& squaddie )
	{
		const auto SPRITE_POSITION_CONSTANT = squaddie.RefrenceSprite().RefrenceSprite().getPosition();
		const auto TO_WORLD_POSITION_CONSTANT = squaddie.GetBoard()->ToWorldCoordinates( to );
		const auto DISPLACEMENT_VECTOR_CONSTANT = TO_WORLD_POSITION_CONSTANT - SPRITE_POSITION_CONSTANT;
		squaddie.RefrenceSprite().SetCurrentDirection( 
				ALL_DIRECTIONS_CONSTANT[ ClosestFacing( from - to ) ] );
		PrintVect( ToUnitVector< int >( from - to ) );
		if( ComparePosition( SPRITE_POSITION_CONSTANT, TO_WORLD_POSITION_CONSTANT ) == false ) {
			squaddie.RefrenceSprite().RefrenceSprite().move( ToUnitVector< float >(
					TO_WORLD_POSITION_CONSTANT - SPRITE_POSITION_CONSTANT ) );
			return false;
		}
		else {
			squaddie.RefrenceBoardPosition() = to;
			return true;
		}
	}

	GameplayManager::GameplayManager( entt::registry& registry_ ) : 
			registry( registry_ ), gameBoard( registry_, 64, 64 ) {}

	Squaddie& GameplayManager::CreateSquaddie( BoardPosition startingPosition )
	{
		Squaddie* newSquaddie = new Squaddie( registry, startingPosition, &gameBoard,
				gameBoard.GetTileWidthConstant(), gameBoard.GetTileHeightConstant() );
		entities.push_back( newSquaddie );
		return *newSquaddie;
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
	void GameplayManager::UpdatePlayer()
	{
		std::list< entt::entity > doneMoving;
		bool allDone = true;
		registry.view< Squaddie::SquaddieRefrence, ORDER_TYPE >().each(
			[ & ]( Squaddie::SquaddieRefrence& squaddie, MoveOrder& order ) {
				if( allDone = ( allDone && order.Tick( squaddie.get() ) ) )
					doneMoving.push_back( squaddie.get().GetID() );
			} );
		for( auto currentEntity : doneMoving )
			registry.remove< ORDER_TYPE >( currentEntity );
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
}