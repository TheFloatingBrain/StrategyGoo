#include "GameplayManager.hpp"

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
		position = &registry.emplace< BoardPosition >( id, start.x, start.y );
		sprite = &registry.emplace< Sprite< 0 > >( id, "Squaddie" );
		registry.emplace< SquaddieRefrence >( id, *this );
		registry.emplace< Updator::UpdatorRefrence >( id, *this );
	}

	sf::Vector2f Squaddie::ToWorldPosition() {
		return ( *board )[ position->x ][ position->y ].ToWorldPosition();
	}

	//Anything that needs to be regularly updated.//
	void Squaddie::Update() {}
	//For animations between orders.//
	void Squaddie::TickOrder() {}

	bool Squaddie::CheckSelect( sf::Vector2f cursorPosition, sf::Vector2f cameraPosition )
	{
		sf::IntRect box = sprite->ObtainCurrentAnimationFrameBounds(
			sprite->GetCurrentDirection(), sprite->GetCurrentFrame() );
		auto worldPosition = ToWorldPosition();
		sf::FloatRect checkMouse = sf::FloatRect{ worldPosition.x,
			worldPosition.y, ( float ) box.width, ( float ) box.height };
		return checkMouse.contains( ( cursorPosition + cameraPosition ) );
	}

	std::optional< entt::entity > Squaddie::SelectSquaddie( entt::registry& registry, sf::View& camera )
	{
		if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
		{
			const sf::Vector2f CURRENT_MOUSE_POSITION_CONSTANT =
					sf::Vector2f( ( float ) sf::Mouse::getPosition().x, ( float ) sf::Mouse::getPosition().y );
			const sf::Vector2f CURRENT_CAMERA_POSITION_CONSTANT =
				sf::Vector2f( ( float ) camera.getViewport().left, ( float ) camera.getViewport().top );
			bool didSelect = false;
			entt::entity selectedID;
			registry.view< SquaddieRefrence >().each( [ & ]( SquaddieRefrence& squaddie ) {
						if( didSelect = ( didSelect || squaddie.get().CheckSelect( 
									CURRENT_MOUSE_POSITION_CONSTANT, CURRENT_CAMERA_POSITION_CONSTANT ) ) )
							selectedID = squaddie.get().id;
					}
			);
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
		const sf::Vector2i MOVEMENT_CONSTANT = ToUnitVector< int >( to - *squaddie.position );
		*squaddie.position += MOVEMENT_CONSTANT;
		return *squaddie.position == to;
	}

	bool MoveOrder::Tick( Squaddie& squaddie )
	{
		const auto SPRITE_POSITION_CONSTANT = squaddie.sprite->GetSprite().getPosition();
		const auto SQUADDIE_WORLD_POSITION_CONSTANT = squaddie.ToWorldPosition();
		if( ComparePosition( SPRITE_POSITION_CONSTANT, SQUADDIE_WORLD_POSITION_CONSTANT ) ) {
			squaddie.sprite->GetSprite().move( ToUnitVector< float >(
				SQUADDIE_WORLD_POSITION_CONSTANT - SPRITE_POSITION_CONSTANT ) );
			return false;
		}
		return true;
	}

	GameplayManager::GameplayManager( entt::registry& registry_ ) : 
			registry( registry_ ), gameBoard( registry_, 64, 64 ) {}

	void GameplayManager::CreateSquaddie( BoardPosition startingPosition )
	{
		entities.push_back( new Squaddie( registry, startingPosition, &gameBoard,
				gameBoard.GetTileWidthConstant(), gameBoard.GetTileHeightConstant() ) );
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

	void GameplayManager::UpdatePlayer() {}
}