#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>

#define CellSize 20
#define Width 1200
#define Height 760

bool gameState = true;

//All direction where player can move
enum Direction
{
	Up,
	Down,
	Left,
	Right
};

struct Vector2
{
private:
	float x, y;
public:
	Vector2(float x, float y) { this->x = x; this->y = y; }
	//getters
	Vector2 getVector2() { return Vector2(x, y); }
	float getX() { return x; }
	float getY() { return y; }
	//setters
	void setVector2(Vector2 newPosition) { x = newPosition.getX(); y = newPosition.getY(); }	
	void setX(float x) { this->x = x; }
	void setY(float y) { this->y = y; }

	//todo:
	//operator overloading
};
struct Object
{
protected:
	Vector2 position = Vector2(0,0);

public:
	virtual void movePosition(Vector2 newPosition) { position = newPosition; }
	virtual Vector2 getPosition() { return position; }
};

struct SnakeBodyPart : public Object
{
public:
	sf::RectangleShape shape;

	SnakeBodyPart()
	{
		shape.setSize(sf::Vector2f(CellSize, CellSize));
		shape.setFillColor(sf::Color::Green);
		shape.setPosition(position.getX(), position.getY());
	}
	void movePosition(Vector2 step)
	{		
		position.setVector2(Vector2(position.getX() + step.getX(), 
			position.getY() + step.getY()));

		if (position.getX() > Width) position.setX(0);
		else if (position.getX() < 0) position.setX(Width);
		if (position.getY() > Height) position.setY(0);
		else if (position.getY() < 0) position.setY(Height);

		shape.setPosition(position.getX(), position.getY());
	}
	void setPosition(Vector2 newPosition)
	{
		position.setVector2(newPosition);
		if (position.getX() > Width) position.setX(0);
		else if (position.getX() < 0) position.setX(Width);
		if (position.getY() > Height) position.setY(0);
		else if (position.getY() < 0) position.setY(Height);
		shape.setPosition(position.getX(), position.getY());
	}
};
class Snake : public Object
{
	std::vector<SnakeBodyPart> body;
	Direction currentDirection;
public:
	Snake()
	{
		currentDirection = Up;
		SnakeBodyPart head;
		head.shape.setFillColor(sf::Color::Yellow);
		head.movePosition(Vector2(CellSize*30, CellSize*15));
		body.push_back(head);
	}
	Vector2 getStep()
	{
		switch (currentDirection)
		{
		case Up:
			return Vector2(0, CellSize);
		case Down:
			return Vector2(0, -CellSize);
		case Left:
			return Vector2(-CellSize, 0);
		case Right:
			return Vector2(CellSize, 0);
		}
	}
	int getSnakeLength() { return body.size(); }
	void movePosition(Vector2 newPosition)
	{
		//moving all body parts without 0
		for (int i = getSnakeLength()-1; i > 0; i--)
			body[i].setPosition(body[i-1].getPosition());

		//moving snake "head"
		body[0].movePosition(newPosition);
	}
	Vector2 getPosition() { return body[0].getPosition(); }
	void addNewBodyPart()
	{
		SnakeBodyPart newBodyPart;
		body.push_back(newBodyPart);
		body[getSnakeLength()-1].movePosition(body[getSnakeLength()-2].getPosition());
	}
	void drawSnakeBody(sf::RenderWindow * window) 
	{
		for (size_t i = 0; i < getSnakeLength(); i++)
			window->draw(body[i].shape);
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(Width, Height), "Snake");//, Style::Fullscreen);
	window.setFramerateLimit(3);
	
	Snake snake;
	snake.addNewBodyPart();
	snake.addNewBodyPart();
	snake.addNewBodyPart();
	std::cout << snake.getSnakeLength();


	while (true)
	{
		std::cout << "Snake step " << snake.getStep().getX() << " " << snake.getStep().getY() << std::endl;
		std::cout << "Snake head position " << snake.getPosition().getX() << " " << snake.getPosition().getY() << std::endl;
		window.clear();
		snake.drawSnakeBody(&window);
		snake.movePosition(snake.getStep());
		window.display();
	}

	return 0;
}