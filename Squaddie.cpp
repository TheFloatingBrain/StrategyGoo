#include "Squaddie.hpp"
#include <iostream>
namespace BioGooContainmentSquad
{
	Squaddie::Squaddie( entt::registry& registry_, BoardPosition start, GameBoard* board_, size_t tileWidth, size_t tileHeight ) :
		Entity( registry_, start, board_, tileWidth, tileHeight, "Squaddie" ) {
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
				if( squaddie.get().CheckSelect( registry, window ) == true && didSelect == false && squaddie.get().active == true ) {
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
}