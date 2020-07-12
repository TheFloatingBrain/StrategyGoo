#include "Player.hpp"
#include <iostream>
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

	float Squaddie::GetSpeed() {
		return speed;
	}
	void Squaddie::SetSpeed( float speed_ ) {
		speed = speed_;
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


	bool MoveOrder::Tick( Squaddie& squaddie, entt::registry& registry )
	{
		const auto SPRITE_POSITION_CONSTANT = squaddie.RefrenceSprite().RefrenceSprite().getPosition();
		const auto TO_WORLD_POSITION_CONSTANT = squaddie.GetBoard()->ToWorldCoordinates( to );
		const auto DISPLACEMENT_VECTOR_CONSTANT = TO_WORLD_POSITION_CONSTANT - SPRITE_POSITION_CONSTANT;
		squaddie.RefrenceSprite().SetCurrentDirection(
			ALL_DIRECTIONS_CONSTANT[ ClosestFacing( from - to ) ] );
		PrintVect( ToUnitVector< int >( from - to ) );
		if( ComparePosition( SPRITE_POSITION_CONSTANT, TO_WORLD_POSITION_CONSTANT ) == false ) {
			auto unit = ToUnitVector< float >( TO_WORLD_POSITION_CONSTANT - SPRITE_POSITION_CONSTANT ).result;
			squaddie.RefrenceSprite().RefrenceSprite().move( unit * squaddie.GetSpeed() );
			return false;
		}
		else {
			MoveEntity< Squaddie::SquaddieRefrence >( registry, squaddie.ObtainThis(), to, squaddie.GetBoard() );
			squaddie.RefrenceBoardPosition() = to;
			return true;
		}
	}
	ShootGrenadeOrder::ShootGrenadeOrder( BoardPosition from_, BoardPosition to_ ) : 
			from( from_ ), to( to_ ) {}

	bool ShootGrenadeOrder::Tick( Squaddie& squaddie, entt::registry& registry )
	{
		bool status = false;
		auto getSpriteComponent = [&]( entt::entity id ) -> Sprite< 0 >& {
			return registry.get< Sprite< 0 > >( id );
		};
		auto getBoardPositionComponent = [&]( entt::entity id ) -> BoardPosition& {
			return registry.get< BoardPosition >( id );
		};
		auto updateSprite = [&]( entt::entity id, BoardPosition whereTo ) {
			getSpriteComponent( id ).RefrenceSprite().setPosition(
				squaddie.GetBoard()->ToWorldCoordinates( whereTo ) );
		};
		auto moveSprite = [&]( entt::entity id, sf::Vector2f displacement ) {
			getSpriteComponent( id ).RefrenceSprite().move( ( float ) displacement.x, ( float ) displacement.y );
			getBoardPositionComponent( id ) = squaddie.GetBoard()->ToBoardCoordinates( 
					getSpriteComponent( id ).RefrenceSprite().getPosition() );
		};
		if( createdGrenade == false )
		{
			std::cout << "A\n";
			grenadeID = registry.create();
			registry.emplace< Sprite< 0 > >( grenadeID, "Grenade" );
			registry.emplace< BoardPosition >( grenadeID, from );
			updateSprite( grenadeID, from );
			createdGrenade = true;
		}
		else if( createdGrenade == true && detonatedGrenade == false )
		{
			std::cout << "B\n";
			auto boardPosition = getBoardPositionComponent( grenadeID );
			PrintVect( boardPosition );
			BoardPosition displacement = to - boardPosition;
			if( displacement == BoardPosition( 0, 0 ) )
			{
				registry.remove_all( grenadeID );
				registry.destroy( grenadeID );
				explosionID = registry.create();
				registry.emplace< Sprite< 0 > >( explosionID, "CubeExplosion1" );
				registry.emplace< BoardPosition >( explosionID, to );
				updateSprite( explosionID, to );
				detonatedGrenade = true;
			}
			else {
				squaddie.RefrenceSprite().SetCurrentDirection( ALL_DIRECTIONS_CONSTANT[ ClosestFacing( -displacement ) ] );
				moveSprite( grenadeID, ToUnitVector( ConvertVector< float, int >( displacement ) ).result * 5.f );
			}
		}
		else if( explosionTime++ >= maxExplosionTime )
		{
			std::cout << "C\n";
			registry.remove_all( explosionID );
			registry.destroy( explosionID );
			status = true;
		}
		return status;
	}
}
