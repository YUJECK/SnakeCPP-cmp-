#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "./Snake.hpp"
#include <vector>

class Game {
private:
	Snake *snake;
	sf::RenderWindow *gameWindow;
	bool stopped;
	sf::Vector2i applePos;
	sf::RectangleShape *apple;
	sf::Clock *tickClock;
	sf::Text *gameOverText;
	float minTimeBetwenTicks;
	bool isGameOver;

	void event(sf::Event *event);
	void draw();
	void update();

public:
	Game();
	~Game();

	sf::Vector2i getApplePos() {return applePos;}
	void genApplePos();
	void cycle();
	void gameOver();
};

#endif