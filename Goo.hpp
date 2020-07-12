#include "Player.hpp"
#ifndef GOO_HEADER_HPP
#define GOO_HEADER_HPP

namespace StrategyGoo
{

	/*struct GooComponent
	{
		BoardPosition& RefrencePosition();
		Sprite< 0 >& RefrenceSprite();
	};*/

	struct Goo : public Updator
	{
		using GooRefrence = std::reference_wrapper< Goo >;
		Goo( entt::registry& registry_, BoardPosition start, 
				GameBoard* board_, size_t tileWidth, size_t tileHeight );
		void Update() override {}
		void UpdateGraphics() override {}
	};
}

#endif