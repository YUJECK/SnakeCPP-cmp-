
#include "./defines.hpp"
#include "./Snake.hpp"
#include "./Game.hpp"

Snake::Snake(Game *game) {
    shape = new sf::RectangleShape();
    shape->setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    shape->setFillColor(sf::Color::Green);

    body = std::vector<sf::Vector2i>();
    body.push_back(sf::Vector2i(0, 0));
    body.push_back(sf::Vector2i(0, 1));
    body.push_back(sf::Vector2i(0, 2));
    body.push_back(sf::Vector2i(0, 3));

    this->game = game;
    
    direction = Down;
    setDirection(Down);
}


void Snake::setDirection(Direction newDirection) {
    switch (newDirection) {
    case Up:
        if (direction == Down) return;
        directionVec = sf::Vector2i(0, -1);
        break;

    case Down:
        if (direction == Up) return;
        directionVec = sf::Vector2i(0, 1);
        break;

    case Left:
        if (direction == Right) return;
        directionVec = sf::Vector2i(-1, 0);
        break;

    case Right:
        if (direction == Left) return;
        directionVec = sf::Vector2i(1, 0);
        break;
    }
    direction = newDirection;
}

void Snake::move() {
    sf::Vector2i head = body[body.size()-1] + directionVec;

    if (WIDTH_CELLS <= head.x || head.x < 0) head.x = (head.x + WIDTH_CELLS) % WIDTH_CELLS;
    if (HEIGHT_CELLS <= head.y || head.y < 0) head.y = (head.y + HEIGHT_CELLS) % HEIGHT_CELLS;

    if (head == game->getApplePos() || std::find(body.begin(), body.end(), game->getApplePos()) != body.end()) game->genApplePos();
    else body.erase(body.cbegin());

    if (std::find(body.begin(), body.end(), head) != body.end()) game->gameOver();
    body.push_back(head);
}

void Snake::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (size_t i = 0; i < body.size(); i++) {
        shape->setPosition((sf::Vector2f)(body[i] * CELL_SIZE));
        target.draw(*shape);
    }
}

Snake::~Snake() {
    delete shape;
}