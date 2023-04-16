#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

enum Direction {
	Up,
	Down,
	Left,
	Right
};

class Game;

class Snake : public sf::Drawable {
private:
	std::vector<sf::Vector2i> body;
	sf::Vector2i directionVec;
	Direction direction;
	sf::RectangleShape *shape;
	Game* game;

public:
	Snake(Game* game);
	~Snake();
	
	void setDirection(Direction newDirection);
	void move();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
#endif