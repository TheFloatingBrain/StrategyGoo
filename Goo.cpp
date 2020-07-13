#include "Goo.hpp"
#include <iostream>
#include <functional>
#include <algorithm>
#include <limits>
#include <random>
#include <chrono>

namespace StrategyGoo
{
	size_t RandomRange( int min, int max )
	{
		std::uniform_int_distribution< int > distribution{ min, max };
		std::mt19937 randomDevice(
			abs( static_cast< int >( std::chrono::system_clock::now().time_since_epoch().count() ) ) );
		return distribution( randomDevice );
	}


	Goo::Goo( entt::registry& registry_, BoardPosition start, GameBoard* board_, size_t tileWidth, size_t tileHeight ) :
			Updator( registry_, start, board_, tileWidth, tileHeight, "CubeGoo1" ) {
		InitilizeRefrences< GooRefrence, Goo >( *this );
		RefrenceSprite().SetActive( false );
		AddGoo( start );
	}

	Goo::GooComponent::GooComponent( entt::registry& registry_, BoardPosition start, GameBoard* board_,
			size_t tileWidth, size_t tileHeight, Goo* parent_ ) : 
			Updator( registry_, start, board_, tileWidth, tileHeight, "CubeGoo1" ), parent( parent_ ) {
		InitilizeRefrences< Goo::GooComponentRefrence, Goo::GooComponent >( *this );
		MakeGooAnimation();
		RefrenceSprite().RefrenceSprite().scale( .63f, .5f );
		RefrenceSprite().RefrenceSprite().setOrigin( 0.f, 16.f );
		RefrenceSprite().SetAnimationRate( 1.f / 5.f );
	}

	void Goo::GooComponent::MakeGooAnimation()
	{
		ANIMATION_TYPE animation;
		for( size_t i = 0; i < AMOUNT_OF_DIRECTIONS_CONSTANT; ++i )
		{
			const auto AMOUNT_OF_FRAMES_FOR_DIRECTION_CONSTANT = RefrenceSprite().ObtainFramesForDirection( ( Direction ) i ).size();
			for( size_t j = 0; j < AMOUNT_OF_FRAMES_FOR_DIRECTION_CONSTANT; ++j )
				RefrenceSprite().RefrenceAnimations()[ 0 ][ 0 ].push_back( ( size_t ) RandomRange( 0, ( int ) AMOUNT_OF_FRAMES_FOR_DIRECTION_CONSTANT - 1 ) );
		}
	}

	Goo::GooComponent& Goo::AddGoo( BoardPosition location )
	{
		GooComponent* newGoo = new GooComponent( 
			registry, location, board, ( size_t ) TILE_WIDTH_CONSTANT, ( size_t ) TILE_HEIGHT_CONSTANT, this );
		goo.push_back( newGoo );
		return *newGoo;
	}

	Goo::EmptySquareLocalType Goo::FindEmptyAround( GooComponent& toLookAround, size_t spreadReach )
	{
		auto position = toLookAround.RefrenceBoardPosition();
		GameBoard& board = *toLookAround.GetBoard();
		EmptySquareLocalType emptySquares;

		auto checkForGoo = [ & ]( Tile& tile ) {
			return toLookAround.RefrenceRegistry().has<
					GooComponentRefrence >( tile.GetID() );
		};

		for( size_t y = ( ( ( size_t ) position.y >= spreadReach ) ? position.y - spreadReach : 0 );
				y <= ( position.y + spreadReach ) && y < board.GetHeight(); ++y )
		{
			for( size_t x = ( ( ( size_t ) position.x >= spreadReach ) ? position.x - spreadReach : 0 );
					x <= ( position.x + spreadReach ) && x < board.GetWidth(); ++x ) {
				if( checkForGoo( board[ x ][ y ] ) == false )
					emptySquares.push_back( BoardPosition( ( int ) x, ( int ) y ) );
			}
		}
		return emptySquares;
	}
	
	Goo::EmptySquareType Goo::FindEmptySquares( Goo& goo, size_t spreadReach )
	{
		Goo::EmptySquareType allEmptySquares;
		std::vector< Goo::GooComponent* >& allGoo = goo.GetGoo();
		for( Goo::GooComponent* currentGoo : allGoo ) {
			allEmptySquares.push_back( std::pair< Goo::GooComponent&, Goo::EmptySquareLocalType >( 
					*currentGoo, Goo::FindEmptyAround( *currentGoo, spreadReach ) ) );
		}
		return allEmptySquares;
	}

	bool Goo::RemoveGoo( GooRefrence& toRemove )
	{
		const size_t AMOUNT_OF_GOO_COMPONENTS_CONSTANT = goo.size();
		for( size_t i = 0; i < AMOUNT_OF_GOO_COMPONENTS_CONSTANT; ++i ) {
			if( goo[ i ]->GetID() == toRemove.get().GetID() ) {
				return RemoveGoo( i );
			}
		}
		return false;
	}

	bool Goo::RemoveGoo( GooComponent& toRemove )
	{
		const size_t AMOUNT_OF_GOO_COMPONENTS_CONSTANT = goo.size();
		for( size_t i = 0; i < AMOUNT_OF_GOO_COMPONENTS_CONSTANT; ++i ) {
			if( goo[ i ]->GetID() == toRemove.GetID() ) {
				return RemoveGoo( i );
			}
		}
		return false;
	}
	bool Goo::RemoveGoo( GooComponent* toRemove )
	{
		const size_t AMOUNT_OF_GOO_COMPONENTS_CONSTANT = goo.size();
		for( size_t i = 0; i < AMOUNT_OF_GOO_COMPONENTS_CONSTANT; ++i ) {
			if( goo[ i ] == toRemove ) {
				return RemoveGoo( i );
			}
		}
		return false;
	}
	bool Goo::RemoveGoo( BoardPosition toRemove )
	{
		const size_t AMOUNT_OF_GOO_COMPONENTS_CONSTANT = goo.size();
		for( size_t i = 0; i < AMOUNT_OF_GOO_COMPONENTS_CONSTANT; ++i ) {
			if( goo[ i ]->RefrenceBoardPosition() == toRemove ) {
				return RemoveGoo( i );
			}
		}
		return false;
	}
	bool Goo::RemoveGoo( size_t toRemove )
	{
		bool status = false;
		if( toRemove < goo.size() )
		{
			status = RemoveEntityFromTile< GooComponentRefrence >( 
					registry, goo[ toRemove ]->RefrenceBoardPosition(), board );
			registry.remove_all( goo[ toRemove ]->GetID() );
			registry.destroy( goo[ toRemove ]->GetID() );
			delete goo[ toRemove ];
			goo.erase( goo.begin() + toRemove );
		}
		return status;
	}

	// * TODO: Add wall avoidence later? Could be done throug exclude or by not allowing it in the rules of FindEmptySquares//
	bool Goo::MoveToward( BoardPosition where, size_t spreadReach, bool grow, std::vector< GooComponent* > exclude )
	{
		if( spreadReach > 0 )
		{
			GooComponent* closest = ClosestToPoint( where, false, exclude );
			GooComponent* furthest = ClosestToPoint( where, true, exclude );
			if( closest != nullptr )
			{

				auto& closestPosition = closest->RefrenceBoardPosition();
				if( where == closestPosition )
					return true;
				if( furthest != nullptr )
				{
					BoardPosition unit = ToUnitVector< int >( where - closest->RefrenceBoardPosition() );
					if( goo.size() == 1 )
					{
						//std::cout << "Closest == Furthest!\n";
						return MoveEntity< GooComponentRefrence >( registry, GooComponentRefrence( *closest ),
								closestPosition, closestPosition += unit, board );
					}
					else
					{
						//std::cout << "Moving!\n";
						auto empty = FindEmptyAround( *closest );
						const size_t AMOUNT_OF_EMPTY_TILES = empty.size();
						if( AMOUNT_OF_EMPTY_TILES <= 0 )
						{
							//std::cout << "NO EMPTY TILES!!\n";
							bool nowhereToMove = true;
							for( auto* excluded : exclude ) {
								for( auto* currentGoo : goo )
									nowhereToMove = ( nowhereToMove && ( currentGoo == excluded ) );
							}
							if( nowhereToMove == true )
								return false;
							exclude.push_back( closest );
							return MoveToward( where, spreadReach, grow, exclude );
						}
						BoardPosition newLocation = empty[ ( size_t ) RandomRange( 0, ( int ) AMOUNT_OF_EMPTY_TILES - 1 ) ];
						auto& furthestPosition = furthest->RefrenceBoardPosition();
						if( grow == true )
							AddGoo( newLocation );
						else if( MoveEntity< GooComponentRefrence >( registry, *furthest, furthestPosition, newLocation, board ) == true )
						{
							//std::cout << "Move made\n";
							furthestPosition = newLocation;
							furthest->RefrenceSprite().RefrenceSprite().setPosition( furthest->ToWorldPosition() );
						}
						else {
							std::cout << "MOVE FAILURE\n";
							return false;
						}
						//std::cout << "Done A\n";
					}
				}
				return MoveToward( where, spreadReach - 1, grow, exclude );
			}
		}
		//std::cout << "Done B!\n";
		return true;
	}

	Goo::GooComponent* Goo::ClosestToPoint( BoardPosition point, bool invert, std::vector< GooComponent* > exclude )
	{
		GooComponent* currentClosest = nullptr;
		if( goo.size() > 0 )
		{
			std::function< bool( size_t, size_t ) > comparison;
			if( invert == false )
				comparison = std::less< size_t >();
			else
				comparison = ( std::greater< size_t >() );
			size_t closestDistance = !invert ? std::numeric_limits< size_t >().max() : 
					std::numeric_limits< size_t >().min();
			size_t currentMagnitude = closestDistance;
			for( auto* currentGoo : goo )
			{
				bool excluded = false;
				for( auto* excludedGoo : exclude )
					excluded = ( excludedGoo == currentGoo );
				currentMagnitude = ( size_t ) Magnitude( point - currentGoo->RefrenceBoardPosition() );
				if( comparison( currentMagnitude, closestDistance ) == true ) {
					currentClosest = currentGoo;
					closestDistance = currentMagnitude;
				}
			}
		}
		return currentClosest;
	}


	std::vector< Goo::GooComponent* >& Goo::GetGoo() {
		return goo;
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

}
