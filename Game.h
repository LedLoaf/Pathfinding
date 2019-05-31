#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include "Grid.h"
#include "Player.h"

const unsigned int ScreenWidth = 1120;
const unsigned int ScreenHeight = 800;
const int TileSize = 32;
const int GridWidth = 300;
const int GridHeight = 300;

class Game {
public:
	Game();

	void processInput();
	void update(float dt=0.012);
	void draw();

	void run();
public:
	void resetFrameTime();
	void updateCulling();
	void cameraBoundaries();
	void playerBoundaries();

	void debugDisplay();
public:
	void drawString(std::string text, sf::Vector2f position, unsigned int size, sf::Color color=sf::Color::White, sf::Color outline = sf::Color::Black);
private:
	sf::RenderWindow window;
	sf::Clock dtClock;
	float dt;
	float frameTime;
	sf::View mainView;
private:
	Player player;
	Grid grid;
	sf::RectangleShape tileSelector;
private:
	int fromX;
	int toX;
	int fromY;
	int toY;
private:
	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2i mousePosView;
	sf::Vector2i mousePosGrid;
private:
	sf::Font font;
	sf::Text text;
	std::stringstream stringStream;
	std::string textString;
	sf::RectangleShape debugBackdrop;
	bool showDebug;
};


