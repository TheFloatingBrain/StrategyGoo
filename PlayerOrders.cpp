#include "PlayerOrders.hpp"
#include <iostream>

namespace BioGooContainmentSquad
{

	MoveOrder::MoveOrder( BoardPosition from_, BoardPosition to_ ) :
		from( from_ ), to( to_ ) {}


	bool MoveOrder::Tick( Squaddie& squaddie, entt::registry& registry )
	{
		const auto SPRITE_POSITION_CONSTANT = squaddie.RefrenceSprite().RefrenceSprite().getPosition();
		const auto TO_WORLD_POSITION_CONSTANT = squaddie.GetBoard()->ToWorldCoordinates( to );
		const auto DISPLACEMENT_VECTOR_CONSTANT = TO_WORLD_POSITION_CONSTANT - SPRITE_POSITION_CONSTANT;
		squaddie.RefrenceSprite().SetCurrentDirection(
			ALL_DIRECTIONS_CONSTANT[ ClosestFacing( from - to ) ] );
		//PrintVect( ToUnitVector< int >( from - to ) );
		if( ComparePosition( SPRITE_POSITION_CONSTANT, TO_WORLD_POSITION_CONSTANT ) == false ) {
			auto unit = ToUnitVector< float >( TO_WORLD_POSITION_CONSTANT - SPRITE_POSITION_CONSTANT ).result;
			squaddie.RefrenceSprite().RefrenceSprite().move( unit * squaddie.GetSpeed() );
			return false;
		}
		else {
			auto& position = squaddie.RefrenceBoardPosition();
			MoveEntity< Squaddie::SquaddieRefrence >( registry, squaddie.ObtainThis(), position, to, squaddie.GetBoard() );
			position = to;
			return true;
		}
	}
	ShootGrenadeOrder::ShootGrenadeOrder( BoardPosition from_, BoardPosition to_ ) :
		from( from_ ), to( to_ ) {}

	FlamethrowerOrder::FlamethrowerOrder( Squaddie& squaddie, sf::Vector2i mousePosition )
	{
		direction = ALL_DIRECTIONS_CONSTANT[
			ClosestFacing( ( mousePosition - squaddie.RefrenceBoardPosition() ) ) ];
		squaddie.RefrenceSprite().SetCurrentDirection(
			( Direction ) ClosestFacing( squaddie.RefrenceBoardPosition() - mousePosition ) );
	}

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
			//std::cout << "A\n";
			grenadeID = registry.create();
			registry.emplace< Sprite< 0 > >( grenadeID, "Grenade" );
			registry.emplace< BoardPosition >( grenadeID, from );
			updateSprite( grenadeID, from );
			createdGrenade = true;
		}
		else if( createdGrenade == true && detonatedGrenade == false )
		{
			//std::cout << "B\n";
			auto boardPosition = getBoardPositionComponent( grenadeID );
			//PrintVect( boardPosition );
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
		else
		{
			if( killedGoo == false )
			{
				Goo* gooParent = nullptr;
				registry.view< Goo::GooComponentRefrence, Tile::TileRefrence >().each(
					[&]( Goo::GooComponentRefrence& goo, Tile::TileRefrence& tile )
					{
						if( goo.get().RefrenceBoardPosition() == to ) {
							gooParent = goo.get().parent;
						}
					}
				);
				if( gooParent != nullptr )
				{
					#ifdef _DEBUG
					std::cout << "Grenade deleting goo\n";
					#endif
					if( gooParent->RemoveGoo( to ) == false ) {
						std::cerr << "ShootGrenadeOrder::Tick( Squaddie& squaddie, "
							"entt::registry& registry ) : bool::Error: Failed to delete goo!\n";
					}
				}
				killedGoo = true;
			}
			if( explosionTime++ >= maxExplosionTime )
			{
				//std::cout << "C\n";
				registry.remove_all( explosionID );
				registry.destroy( explosionID );
				status = true;
			}
		}
		return status;
	}

	void FlamethrowerOrder::KillGoo( Squaddie& squaddie, entt::registry& registry )
	{
		auto squaddiePosition = squaddie.RefrenceBoardPosition();
		auto directionVector = DIRECTION_VECTOR_FACINGS_CONSTANT[ ( size_t ) direction ];
		std::vector< Goo::GooComponentRefrence > toRemove;
		registry.view< Goo::GooComponentRefrence, Tile::TileRefrence >().each(
			[&]( Goo::GooComponentRefrence& goo, Tile::TileRefrence& tile )
			{
				for( size_t i = 1; i <= maxFlameLength; ++i )
				{
					if( goo.get().RefrenceBoardPosition() ==
						( squaddiePosition + ( directionVector * ( ( int ) i ) ) ) )
						toRemove.push_back( goo );
				}
				//auto deltaPosition = goo.get().RefrenceBoardPosition() - squaddiePosition;
				//if( ALL_DIRECTIONS_CONSTANT[ ClosestFacing( deltaPosition ) ] == direction &&
				//	ConditionalMagnitude( deltaPosition ) < maxFlameLength )
				//	toRemove.push_back( goo );
			}
		);
		for( auto currentGoo : toRemove )
		{
			#ifdef _DEBUG
			std::cout << "Flamethrower deleting goo\n";
			#endif
			Goo* gooParent = currentGoo.get().parent;
			if( gooParent->RemoveGoo( currentGoo.get() ) == false ) {
				std::cerr << "ShootGrenadeOrder::Tick( Squaddie& squaddie, "
					"entt::registry& registry ) : bool::Error: Failed to delete goo!\n";
			}
		}
		killedGoo = true;
	}

	void FlamethrowerOrder::CreateFlameSprite( Squaddie& squaddie, entt::registry& registry )
	{
		FlameSpriteType& flames = registry.get< FlameSpriteType >( flameID );
		auto directionVector = DIRECTION_VECTOR_FACINGS_CONSTANT[ ( size_t ) direction ];
		GameBoard* squaddieGameBoard = squaddie.GetBoard();
		sf::IntRect confiningRectangle{ 0, 0, ( int ) squaddieGameBoard->GetWidth(), ( int ) squaddieGameBoard->GetHeight() };
		const auto RESULTING_BOARD_POSITION_CONSTANT =
			squaddie.RefrenceBoardPosition() + ( directionVector * ( int ) ( ( flames.size() + 1 ) ) );
		if( confiningRectangle.contains( RESULTING_BOARD_POSITION_CONSTANT ) )
		{
			flames.push_back( Sprite< 0 >( "CubeExplosion1" ) );
			( *( --flames.end() ) ).RefrenceSprite().setPosition(
				squaddie.GetBoard()->ToWorldCoordinates( RESULTING_BOARD_POSITION_CONSTANT ) );
		}
	}

	bool FlamethrowerOrder::Tick( Squaddie& squaddie, entt::registry& registry )
	{
		if( killedGoo == false )
		{
			KillGoo( squaddie, registry );
			flameID = registry.create();
			registry.emplace< FlameSpriteType >( flameID );
		}
		FlameSpriteType& flames = registry.get< FlameSpriteType >( flameID );
		const size_t CURRENT_FLAME_LENGTH_CONSTANT = flames.size();
		if( ( flameTicks % flameTicksPerFlame ) == 0 )
		{
			if( ( flameTicks / flameTicksPerFlame ) == CURRENT_FLAME_LENGTH_CONSTANT &&
				CURRENT_FLAME_LENGTH_CONSTANT < maxFlameLength )
				CreateFlameSprite( squaddie, registry );
			else if( ( 2 * flameTicks / flameTicksPerFlame ) == ( 2 * CURRENT_FLAME_LENGTH_CONSTANT ) &&
				CURRENT_FLAME_LENGTH_CONSTANT >= 0 )
				flames.pop_back();
			else if( flameTicks >= ( 2 * flameTicksPerFlame * maxFlameLength ) )
			{
				registry.remove_all( flameID );
				registry.destroy( flameID );
				return true;
			}
		}
		++flameTicks;
		return false;
	}

}
