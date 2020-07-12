#include "Player.hpp"
namespace StrategyGoo
{
	Updator::Updator( entt::registry& registry_, GameBoard* board_, 
			size_t tileWidth, size_t tileHeight ) : registry( registry_ ), board( board_ ),
			ENTITY_TILE_WIDTH_CONSTANT( tileWidth ), ENTITY_TILE_HEIGHT_CONSTANT( tileHeight ) {}

	BoardPosition& Updator::RefrenceBoardPosition() {
		return registry.get< BoardPosition >( id );
	}
	Sprite< 0 >& Updator::RefrenceSprite() {
		return registry.get< Sprite< 0 > >( id );
	}

	GameBoard* Updator::GetBoard() {
		return board;
	}
	entt::entity Updator::GetID() {
		return id;
	}

	sf::Vector2f Updator::ToWorldPosition() {
		auto position = RefrenceBoardPosition();
		return sf::Vector2f( ( float ) position.x * ENTITY_TILE_WIDTH_CONSTANT,
			( float ) position.y * ENTITY_TILE_WIDTH_CONSTANT );
	}

	Squaddie::Squaddie( entt::registry& registry_, BoardPosition start, GameBoard* board_, size_t tileWidth, size_t tileHeight ) : 
		Updator( registry_, board_, tileWidth, tileHeight )
	{
		id = registry.create();
		registry.emplace< BoardPosition >( id, start.x, start.y );
		registry.emplace< Sprite< 0 > >( id, "Squaddie" );
		RefrenceSprite().RefrenceSprite().setPosition( ToWorldPosition() );
		registry.emplace< SquaddieRefrence >( id, *this );
		registry.emplace< Updator::UpdatorRefrence >( id, *this );
	}

	//Anything that needs to be regularly updated.//
	void Squaddie::Update() {}
	//For animations between orders.//
	void Squaddie::TickOrder() {}

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
			registry.view< SquaddieRefrence >().each( [&]( SquaddieRefrence& squaddie ) {
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
}
