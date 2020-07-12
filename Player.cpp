#include "Player.hpp"
namespace StrategyGoo
{
	Squaddie::Squaddie( entt::registry& registry_, BoardPosition start, GameBoard* board_, size_t tileWidth, size_t tileHeight ) :
			Updator( registry_, start, board_, tileWidth, tileHeight, "Squaddie" ) {
		InitilizeRefrences< SquaddieRefrence, Squaddie >( *this );
	}

	Squaddie& Squaddie::ObtainThis() {
		return *this;
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

	bool MoveOrder::Execute( Squaddie& squaddie, entt::registry& registry )
	{
		auto position = squaddie.RefrenceBoardPosition();
		const sf::Vector2i MOVEMENT_CONSTANT = ToUnitVector< int >( to - position );
		position += MOVEMENT_CONSTANT;
		return position == to;
	}

	bool MoveOrder::Tick( Squaddie& squaddie, entt::registry& registry )
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
			MoveEntity< Squaddie::SquaddieRefrence >( registry, squaddie.ObtainThis(), to, squaddie.GetBoard() );
			squaddie.RefrenceBoardPosition() = to;
			return true;
		}
	}
}
