#include "Player.hpp"
#ifndef GOO_HEADER_HPP
#define GOO_HEADER_HPP

namespace StrategyGoo
{
	size_t RandomRange( int min, int max );

	struct Goo : public Updator
	{
		using GooRefrence = std::reference_wrapper< Goo >;
		Goo( entt::registry& registry_, BoardPosition start, 
				GameBoard* board_, size_t tileWidth, size_t tileHeight );
		struct GooComponent : Updator
		{
			Goo* parent;
			GooComponent( entt::registry& registry_, BoardPosition start,
					GameBoard* board_, size_t tileWidth, size_t tileHeight, Goo* parent_ );
			void Update() override {}
			void UpdateGraphics() override {}
		};
		using GooComponentRefrence = std::reference_wrapper< GooComponent >;
		void Update() override {}
		void UpdateGraphics() override {}

		GooComponent& AddGoo( BoardPosition location );

		bool RemoveGoo( GooRefrence& toRemove );
		bool RemoveGoo( GooComponent& toRemove );
		bool RemoveGoo( GooComponent* toRemove );
		bool RemoveGoo( BoardPosition toRemove );
		bool RemoveGoo( size_t toRemove );


		using EmptySquareLocalType = std::vector< BoardPosition >;
		using EmptySquareType = std::vector< std::pair< GooComponentRefrence, EmptySquareLocalType > >;

		static EmptySquareLocalType FindEmptyAround( GooComponent& toLookAround, size_t spreadReach = 1 );
		static EmptySquareType FindEmptySquares( Goo& goo, size_t spreadReach = 1 );

		bool MoveToward( BoardPosition where, size_t spreadReach = 1, bool grow = false, std::vector< GooComponent* > exclude = {} );

		GooComponent* ClosestToPoint( BoardPosition point, bool invert = false, std::vector< GooComponent* > exclude = {} );

		std::vector< GooComponent* >& GetGoo();
		protected:
			std::vector< GooComponent* > goo;
	};
}

#endif