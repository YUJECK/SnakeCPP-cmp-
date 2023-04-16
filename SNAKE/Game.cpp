#include <iostream>
#include "./Game.hpp"
#include "./defines.hpp"

Game::Game() {
	snake = new Snake(this); 
	gameWindow = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Snake");
	stopped = false;
	apple = new sf::RectangleShape();
	apple->setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
	apple->setFillColor(sf::Color::Red);

	tickClock = new sf::Clock();
	tickClock->getElapsedTime().asSeconds();

	minTimeBetwenTicks = 1.f / 5; // 5 тика в секунду, мне лень выделять под это отдельную функцию.
	isGameOver = false;

	sf::Font *font = new sf::Font();
	if (!font->loadFromFile("font.ttf")) {
		std::cout << "Где шрифт, бесы?" << std::endl;
		exit(1);
	}

	gameOverText = new sf::Text("Game over!", *font);

	genApplePos();
}

Game::~Game() {
	delete apple;
	delete snake;
	delete gameOverText;
	delete gameWindow;
	delete tickClock;
}

void Game::update() {
	if (tickClock->getElapsedTime().asSeconds() >= minTimeBetwenTicks) {
		tickClock->restart();
		snake->move();
	}
}

void Game::draw() {
	gameWindow->clear();
	gameWindow->draw(*apple);
	gameWindow->draw(*snake);
	if (isGameOver) gameWindow->draw(*gameOverText);
	gameWindow->display();
}

void Game::event(sf::Event *event) {
	if ((event->type == sf::Event::KeyPressed) & (event->key.code == sf::Keyboard::W))
		snake->setDirection(Up);
	if ((event->type == sf::Event::KeyPressed) & (event->key.code == sf::Keyboard::S))
		snake->setDirection(Down);
	if ((event->type == sf::Event::KeyPressed) & (event->key.code == sf::Keyboard::A))
		snake->setDirection(Left);
	if ((event->type == sf::Event::KeyPressed) & (event->key.code == sf::Keyboard::D))
		snake->setDirection(Right);
	if (event->type == sf::Event::Closed) {
		gameWindow->close();
		stopped = true;
	}
}

void Game::genApplePos() {
	applePos = sf::Vector2i(rand() % WIDTH_CELLS, rand() % HEIGHT_CELLS);
	apple->setPosition((sf::Vector2f)(applePos * CELL_SIZE));
}

void Game::cycle() {
	while (true) {
		for (sf::Event event; gameWindow->pollEvent(event);) this->event(&event);
		if (stopped) break;
		if (!isGameOver) update();
		draw();
	}
}

void Game::gameOver() {
	isGameOver = true;
}

int main() {
	Game game = Game();
	game.cycle();

	return 0;
}