#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>

#define BLOCK 20
#define X 1340
#define Y 760

bool PlayGame = true;

using namespace sf;

enum NAPRAVLENIE
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
class block
{
public:
	RectangleShape shape;
	int x, y;
	NAPRAVLENIE to;

	block()
	{
		to = LEFT;		
		shape.setSize(Vector2f(BLOCK, BLOCK));
		shape.setFillColor(Color::Green);
		x = BLOCK * 10;
		y = BLOCK * 10;
		shape.setPosition(x, y);
	}
};
class Snake
{
public:
	int size = 2;
	int snake_size = 3;
	std::vector<block> body;
	NAPRAVLENIE old_to;

	Snake()
	{
		body.resize(3);
	}
	void Move()
	{
		for (int i = size; i > 0; i--)
		{
			body[i].x = body[i - 1].x;
			body[i].y = body[i - 1].y;
			body[i].shape.setPosition(body[i].x, body[i].y);
		}
		if (body[0].to == LEFT)
		{
			if (old_to == RIGHT)
			{
				body[0].x += BLOCK;
			}
			else
			{
				body[0].x -= BLOCK;
				old_to = LEFT;
			}
		}
		if (body[0].to == RIGHT)
		{
			if (old_to == LEFT)
			{
				body[0].x -= BLOCK;
			}
			else
			{
				body[0].x += BLOCK;
				old_to = RIGHT;
			}

		}
		if (body[0].to == DOWN)
		{
			if (old_to == UP)
			{
				body[0].y -= BLOCK;
			}
			else
			{
				body[0].y += BLOCK;
				old_to = DOWN;
			}
		}
		if (body[0].to == UP)
		{
			if (old_to == DOWN)
			{
				body[0].y += BLOCK;
			}
			else
			{
				body[0].y -= BLOCK;
				old_to = UP;
			}
		}

		body[0].shape.setPosition(body[0].x, body[0].y);

		if (body[0].y > Y)
		{
			body[0].y = 0;
		}
		if (body[0].y < 0)
		{
			body[0].y = Y;
		}
		if (body[0].x > X)
		{
			body[0].x = 0;
		}
		if (body[0].x < 0)
		{
			body[0].x = X;
		}
	}  
	void isDead()
	{
		for (size_t i = 1; i <= size; i++)
		{
			if (body[0].x == body[i].x and body[0].y == body[i].y)
			{
				PlayGame = false;
			}
		}
	}
};
class Apple
{
public:
	int applesEat = 0;
	int score = 0;
	bool isYellowApple = false;
	bool isGreenApple = false;
	CircleShape shape;
	int x, y;

	Apple()
	{
		x = 0;
		y = 0;
		shape.setRadius(10.f);
		shape.setFillColor(Color::Red);
	}
	void Spawn()
	{
		x = rand() % (X / BLOCK);
		y = rand() % (Y / BLOCK);
		x *= BLOCK;
		y *= BLOCK;
		shape.setPosition(x, y);
	}
	bool isEat(RectangleShape snake)
	{
		Vector2f snake_position = snake.getPosition();
		int snake_x = snake_position.x;
		int snake_y = snake_position.y; 

		if (x == snake_x and y == snake_y and isYellowApple == false and isGreenApple == false)
		{
			Spawn();
			applesEat++;
			score++;
			return true;
		}
		if (x == snake_x and y == snake_y and isYellowApple == true)
		{
			score += 2;
			isYellowApple = false;
			shape.setFillColor(Color::Red);
			Spawn();
		}
		if (x == snake_x and y == snake_y and isGreenApple == true)
		{
			score -= 1;
			isGreenApple = false;
			shape.setFillColor(Color::Red);
			Spawn();
		}
		else
		{
			return false;
		}
	}
};

int main()
{
	srand(time(NULL));
	Font font;
	Apple apple;
	Snake snake;
	int fps = 5;
	int a = apple.applesEat;
	std::string applesEat;
	font.loadFromFile("font.ttf");
	Text GameOver("GAME OVER", font, 100);
	GameOver.setStyle(Text::Bold);
	GameOver.setOrigin(GameOver.getLocalBounds().width / 2, GameOver.getLocalBounds().height / 2);
	GameOver.setPosition(X / 2, Y / 2);
	RenderWindow window(VideoMode(X, Y), "SNAKE");//, Style::Fullscreen);
	font.loadFromFile("font.ttf");
	Text text("", font, 70);
	text.setStyle(Text::Bold);
	applesEat += std::to_string(a);
	text.setString(applesEat);
	text.setPosition(10, 0);
	window.setFramerateLimit(fps);
	apple.Spawn();
	bool isMenu = true;

	while (window.isOpen())
	{
		Event event;
		
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed && event.key.code == (Keyboard::Escape))
			{
				window.close();
			}
			if (event.type == Event::KeyPressed && event.key.code == (Keyboard::Down))
			{
				snake.body[0].to = DOWN;
			}
			if (event.type == Event::KeyPressed && event.key.code == (Keyboard::Up))
			{
				snake.body[0].to = UP;
			}
			if (event.type == Event::KeyPressed && event.key.code == (Keyboard::Left))
			{
				snake.body[0].to = LEFT;
			}
			if (event.type == Event::KeyPressed && event.key.code == (Keyboard::Right))
			{
				snake.body[0].to = RIGHT;
			}
		}
		if (PlayGame)
		{
			a = apple.score;
			applesEat = "";
			applesEat += std::to_string(a);
			text.setString(applesEat);
			apple.isEat(snake.body[0].shape);
			window.clear();
			window.draw(text);
			window.draw(apple.shape);
			snake.Move();
			snake.isDead();
			for (int i = 0; i <= snake.size; i++)
			{
				window.draw(snake.body[i].shape);
			}
		}
	
		if (!PlayGame)
		{
			window.draw(GameOver);
		}
		window.display();
		if (PlayGame)
		{
			if (apple.isEat(snake.body[0].shape) == true)
			{
				int GreenApple = rand() % 5;
				int YellowApple = rand() % 5;
				snake.size++;
				snake.snake_size++;
				snake.body.push_back(snake.body[0]);
				snake.body[snake.size].x = snake.body[snake.snake_size - 1].x - BLOCK;
				snake.body[snake.size].y = snake.body[snake.snake_size - 1].y;
				snake.body[snake.size].shape.setPosition(snake.body[snake.size].x, snake.body[snake.size].y);
				if (YellowApple == 1)
				{
					apple.shape.setFillColor(Color::Yellow);
					apple.isYellowApple = true;
					apple.Spawn();
				}
				if (GreenApple == 1)
				{	
					apple.isGreenApple = true;
					apple.shape.setFillColor(Color::Green);
					apple.Spawn();
				}
				if (apple.applesEat % 2 == 0)
				{
					fps++;
					window.setFramerateLimit(fps);
				}
			}
		}
	}
	return 0;
}