#ifndef HPBAR_H
#define HPBAR_H

#include <SFML\Graphics.hpp>

class enemy;
class player;

class HPbar : public sf::RectangleShape
{
public :
	void init(double maxHP, double widthSize, double HeightSize, sf::Color startingColor);

	void update(sf::Vector2f position, double actualHP, bool centerRectangle, double radius = 0);

protected:
	double maxHP;
	double startingWidth;
	double height;
	double startingRCol, startingGCol, startingBCol;

};
#endif // !1
