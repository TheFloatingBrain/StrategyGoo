#include "GameBoard.hpp"
#include <math.h>

#ifndef VECTOR_UTILITY_HEADER_HPP
#define VECTOR_UTILITY_HEADER_HPP
namespace StrategyGoo
{

	template< typename SCALER_TYPE = int >
	constexpr sf::Rect< SCALER_TYPE > RectangleFromVectors( 
			sf::Vector2< SCALER_TYPE > position, sf::Vector2< SCALER_TYPE > dimentions ) {
		return sf::Rect< SCALER_TYPE >( position.x, position.y, dimentions.x, dimentions.y );
	}

	template< typename SCALER_0_TYPE, typename SCALER_1_TYPE >
	constexpr sf::Rect< SCALER_0_TYPE > ConvertRectangle( sf::Rect< SCALER_1_TYPE > rectangle ) {
		return sf::Rect< SCALER_0_TYPE >{ ( SCALER_0_TYPE ) rectangle.left, ( SCALER_0_TYPE ) rectangle.right, 
				( SCALER_0_TYPE ) rectangle.width, ( SCALER_0_TYPE ) rectangle.height };
	}

	template< typename SCALER_TYPE = float >
	constexpr double Magnitude( sf::Vector2< SCALER_TYPE > of ) {
		return sqrt( ( of.x * of.x ) + ( of.y * of.y ) );
	}

	template< typename SCALER_0_TYPE, typename SCALER_1_TYPE >
	constexpr sf::Vector2< SCALER_0_TYPE > ConvertVector( sf::Vector2< SCALER_1_TYPE > vector ) {
		return sf::Vector2< SCALER_0_TYPE >{ ( SCALER_0_TYPE ) vector.x, ( SCALER_0_TYPE ) vector.y };
	}

	template< typename SCALER_TYPE >
	constexpr sf::Vector2< SCALER_TYPE > RectanglePosition( sf::Rect< SCALER_TYPE > from ) {
		return sf::Vector2< SCALER_TYPE >( from.left, from.top );
	}

	bool ComparePosition( sf::Vector2f first, sf::Vector2f second );

	template< typename SCALER_TYPE = float >
	struct ToUnitVector
	{
		sf::Vector2< SCALER_TYPE > result;
		ToUnitVector( sf::Vector2< SCALER_TYPE > vector )
		{
			const double MAGNITUDE_CONSTANT = Magnitude( vector );
			result = sf::Vector2< SCALER_TYPE >(
				vector.x / ( SCALER_TYPE ) MAGNITUDE_CONSTANT, vector.y / ( SCALER_TYPE ) MAGNITUDE_CONSTANT );
		}
		operator sf::Vector2< SCALER_TYPE >() {
			return result;
		}
	};

	size_t ClosestFacing( sf::Vector2i vector );


	void PrintRect( sf::FloatRect rect );
	void PrintRect( sf::IntRect rect );
	void PrintVect( sf::Vector2f v );
	void PrintVect( sf::Vector2i v );

}
#endif
