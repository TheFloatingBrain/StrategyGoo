#include "Player.hpp"
#ifndef GOO_HEADER_HPP
#define GOO_HEADER_HPP

namespace StrategyGoo
{
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

		using EmptySquareLocalType = std::list< BoardPosition >;
		using EmptySquareType = std::vector< std::pair< GooComponentRefrence, EmptySquareLocalType > >;

		static EmptySquareLocalType FindEmptyAround( GooComponent& toLookAround, size_t spreadReach = 1 );
		static EmptySquareType FindEmptySquares( Goo& goo, size_t spreadReach = 1 );

		std::vector< GooComponent* >& GetGoo();
		protected:
			std::vector< GooComponent* > goo;
	};
}

#endif