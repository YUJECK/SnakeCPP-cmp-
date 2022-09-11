#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>

#define CellSize 20
#define Width 1200
#define Height 760

//all direction where player can move
enum Direction
{
	Up,
	Down,
	Left,
	Right
};

//simple x,y in one class
class Vector2
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
class Object
{
protected:
	Vector2 position = Vector2(0,0);

public:
	virtual void movePosition(Vector2 newPosition) { position = newPosition; }
	virtual Vector2 getPosition() { return position; }
};
class SnakeBodyPart : public Object
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

//static classes
class Snake : Object
{
private:
	std::vector<SnakeBodyPart> body;
	Direction currentDirection;
public:
	Snake()
	{
		currentDirection = Up;
		SnakeBodyPart head;
		head.shape.setFillColor(sf::Color::Yellow);
		head.movePosition(Vector2(CellSize * 30, CellSize * 15));
		body.push_back(head);
		addNewBodyPart();
		addNewBodyPart();
		addNewBodyPart();
	}

	//getters
	Vector2 getStep()
	{
		switch (currentDirection)
		{
		case Up:
			return Vector2(0, -CellSize);
		case Down:
			return Vector2(0, CellSize);
		case Left:
			return Vector2(-CellSize, 0);
		case Right:
			return Vector2(CellSize, 0);
		}
	}
	Vector2 getPosition() { return body[0].getPosition(); }
	Direction getDirection() { return currentDirection; }
	int getSnakeLength() { return body.size(); }
	//setters
	void setDirection(Direction newDirection) { currentDirection = newDirection; }
	//other
	void movePosition(Vector2 newPosition)
	{
		//moving all body parts without 0
		for (int i = getSnakeLength() - 1; i > 0; i--)
			body[i].setPosition(body[i - 1].getPosition());

		//moving snake "head"
		body[0].movePosition(newPosition);
	}
	void addNewBodyPart()
	{
		SnakeBodyPart newBodyPart;
		body.push_back(newBodyPart);
		body[getSnakeLength() - 1].movePosition(body[getSnakeLength() - 2].getPosition());
	}
	void drawSnakeBody(sf::RenderWindow& window)
	{
		for (size_t i = 0; i < getSnakeLength(); i++)
			window.draw(body[i].shape);
	}
};
class GameController
{
private:
	sf::RenderWindow* gameWindow;
	Snake* snake;
	int defaultFPS;
	int currentFPS;

public:
	GameController(Snake& snake, sf::RenderWindow& gameWindow,int fps) 
	{
		this->snake = &snake;
		defaultFPS = fps; 
		this->gameWindow = &gameWindow;
		setCurrentFPSToDefault();
	}
	void update()
	{
		gameWindow->clear();
		snake->drawSnakeBody(*gameWindow);
		snake->movePosition(snake->getStep());
		gameWindow->display();
	}

	//getters
	int getCurrentFPS() { return currentFPS; }
	int getDefaultFPS() { return defaultFPS; }
	//setters
	void setGameWindow(sf::RenderWindow& newGameWindow) { gameWindow = &newGameWindow; }
	void setCurrentFPS(int newFPS)
	{
		currentFPS = newFPS;
		gameWindow->setFramerateLimit(currentFPS);
	}
	void setCurrentFPSToDefault()
	{
		currentFPS = defaultFPS;
		gameWindow->setFramerateLimit(currentFPS);
	}
};
class InputController
{
private:
	Snake * snake;
	sf::RenderWindow * gameWindow;
public:
	InputController(Snake &snake, sf::RenderWindow &gameWindow) 
	{
		this->snake = &snake; 
		this->gameWindow = &gameWindow;
	}
	void update()
	{
		sf::Event event;
		if (gameWindow->pollEvent(event))
		{
			if ((event.type == sf::Event::KeyPressed) & (event.key.code == sf::Keyboard::W))
				snake->setDirection(Up);
			if ((event.type == sf::Event::KeyPressed) & (event.key.code == sf::Keyboard::S))
				snake->setDirection(Down);
			if ((event.type == sf::Event::KeyPressed) & (event.key.code == sf::Keyboard::A))
				snake->setDirection(Left);
			if ((event.type == sf::Event::KeyPressed) & (event.key.code == sf::Keyboard::D))
				snake->setDirection(Right);
			if (event.type == sf::Event::Closed)
				gameWindow->close();
		}
	}
};

int main()
{	
	sf::RenderWindow gameWindow(sf::VideoMode(Width, Height), "Snake");
	Snake snake;
	GameController gameController(snake, gameWindow, 10);
	InputController inputController(snake, gameWindow);

	while (true)
	{
		gameController.update();
		inputController.update();
	}

	return 0;
}