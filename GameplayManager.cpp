#include "GameplayManager.hpp"

	#include <iostream>
namespace StrategyGoo
{
	bool ComparePosition( sf::Vector2f first, sf::Vector2f second )
	{
		return ( abs( first.x - second.x ) < FLT_EPSILON ) &&
				( abs( first.y - second.y ) < FLT_EPSILON );
	}

	Squaddie::Squaddie( entt::registry& registry_, BoardPosition start, GameBoard* board_, size_t tileWidth, size_t tileHeight ) :
		registry( registry_ ), board( board_ ), ENTITY_TILE_WIDTH_CONSTANT( tileWidth ), ENTITY_TILE_HEIGHT_CONSTANT( tileHeight ) 
	{
		id = registry.create();
		registry.emplace< BoardPosition >( id, start.x, start.y );
		registry.emplace< Sprite< 0 > >( id, "Squaddie" );
		RefrenceSprite().RefrenceSprite().setPosition( ToWorldPosition() );
		std::cout << "Squaddie Constructor " << ToWorldPosition().x << ", " << ToWorldPosition().y << "\n";
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

	bool Squaddie::CheckSelect( entt::registry& registry, sf::RenderWindow& window )
	{
		sf::IntRect box = RefrenceSprite().GetSprite().getTextureRect();
		auto worldPosition = ToWorldPosition();
		sf::IntRect checkMouse{ ( int ) worldPosition.x,
			( int ) worldPosition.y, box.width, box.height };
		return checkMouse.contains( ( sf::Mouse::getPosition( window ) + 
				ConvertVector< int, float >( RectanglePosition( window.getView().getViewport() ) ) ) );
	}

	std::optional< entt::entity > Squaddie::SelectSquaddie( entt::registry& registry, sf::RenderWindow& window )
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
			if( didSelect == true )
				return std::optional< entt::entity >( selectedID );
		}
		return std::nullopt;
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

	bool MoveOrder::Move( Squaddie& squaddie )
	{
		auto position = squaddie.RefrenceBoardPosition();
		const sf::Vector2i MOVEMENT_CONSTANT = ToUnitVector< int >( to - position );
		position += MOVEMENT_CONSTANT;
		return position == to;
	}

	bool MoveOrder::Tick( Squaddie& squaddie )
	{
		const auto SPRITE_POSITION_CONSTANT = squaddie.RefrenceSprite().RefrenceSprite().getPosition();
		const auto SQUADDIE_WORLD_POSITION_CONSTANT = squaddie.ToWorldPosition();
		if( ComparePosition( SPRITE_POSITION_CONSTANT, SQUADDIE_WORLD_POSITION_CONSTANT ) ) {
			squaddie.RefrenceSprite().RefrenceSprite().move( ToUnitVector< float >(
				SQUADDIE_WORLD_POSITION_CONSTANT - SPRITE_POSITION_CONSTANT ) );
			return false;
		}
		return true;
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

	void GameplayManager::UpdatePlayer() {
	}

	entt::registry& GameplayManager::RefrenceRegistry() {
		return registry;
	}
}