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
		//struct GooComponent : Updator
		//{
		//	Goo* parent;
			//GooComponent( entt::registry& registry_, BoardPosition start,
			//		GameBoard* board_, size_t tileWidth, size_t tileHeight, Goo parent_ );
		//};
		void Update() override {}
		void UpdateGraphics() override {}
	};
}

#endif