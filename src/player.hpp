#pragma once
#include <SFML\Graphics.hpp>
#include "entity.hpp"
#include "HPBar.h"




class player : public sf::Sprite, public aliveCreature
{
public:
	player(float radius, sf::Color color, sf::Vector2f position, sf::RenderWindow &Window , std::vector<std::vector<enemy*>> &enemyArrayRef);
	void update(sf::Vector2i mousePosition, sf::Vector2f windowOffset);
	virtual void dealDamage(damageParameters DmgParams) override;
	void draw();


	/***************************************************/
	//			Settery
	void setNewPlayerParams(playerParameters newParams)
	{
		this->parameters = newParams;
	}
	void setCircleFillColor(sf::Color col)
	{
		this->circle.setFillColor(col);
	}
	void setHP(double val)
	{
		if (val > this->parameters.maxHP)
			val = this->parameters.maxHP;
		if (val < 0)
			val = 0;

		this->HP = val;
	}
	void setTextFont(sf::Font &font)
	{
		this->HPText.setFont(font);
	}

	/***************************************************/



	/***************************************************/
	//			Gettery
	HPbar	getHPbar()
	{
		return this->RectHPOfPlayer;
	}
	sf::CircleShape getCircle()
	{
		return circle;
	}
	sf::VertexArray getLine() {
		return SightLine;
	}
	sf::Text		getText()
	{
		return this->HPText;
	}
	double getHP()
	{
		return this->HP;
	}
	double getVx() {
		return movementSpeed;
	}
	double getVy()
	{
		return movementSpeed;
	}
	double getAngle()
	{
		return angle;
	}
	sf::Vector2f getCenter()
	{
		// jako ¿e ustawiliœmy wczeœniej origin center na œrodek
		// to nie musimy dodawaæ do pozycji promienia 
		return sf::Vector2f(circle.getPosition().x, circle.getPosition().y );
	}
	playerParameters getParameters() const
	{
		return this->parameters;
	}
	/***************************************************/

private:
	void initializeStartupParameters();
	void manageSpecialAbilities();
	void manageKeyboardInput();

	sf::RenderWindow &refToMainWindow;
	std::vector<std::vector<enemy*>> &enemyArrayRef;


	double prevHP;
	double	armor;
	double movementSpeed, angle;
	playerParameters parameters;
	HPbar RectHPOfPlayer;
	sf::CircleShape  circle;
	sf::Texture      texture;
	sf::VertexArray  SightLine;
	sf::Text		 HPText;


};
