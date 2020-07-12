#include "VectorUtility.hpp"
#ifdef _DEBUG
#include <iostream>
#endif
namespace StrategyGoo
{
	bool ComparePosition( sf::Vector2f first, sf::Vector2f second ) {
		return ( abs( first.x - second.x ) < 1 ) &&
			( abs( first.y - second.y ) < 1 );
	}

	size_t ClosestFacing( sf::Vector2i vector )
	{
		sf::Vector2i unitVector = ToUnitVector< int >{ vector };
		for( size_t i = 0; i < AMOUNT_OF_DIRECTIONS_CONSTANT; ++i ) {
			if( DIRECTION_VECTOR_FACINGS_CONSTANT[ i ] == unitVector )
				return i;
		}
		return 0;
	}

	void PrintRect( sf::FloatRect rect )
	{
		#ifdef _DEBUG
		std::cout << rect.left << ", " << rect.top <<
			", " << rect.width << ", " << rect.height << "\n";
		#endif
	}
	void PrintRect( sf::IntRect rect )
	{
		#ifdef _DEBUG
		std::cout << rect.left << ", " << rect.top <<
			", " << rect.width << ", " << rect.height << "\n";
		#endif
	}
	void PrintVect( sf::Vector2f v )
	{
		#ifdef _DEBUG
		std::cout << v.x << ", " << v.y << "\n";
		#endif
	}
	void PrintVect( sf::Vector2i v )
	{
		#ifdef _DEBUG
		std::cout << v.x << ", " << v.y << "\n";
		#endif
	}
}
