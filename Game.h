#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include "Grid.h"
#include "Player.h"
#include "Enemy.h"
#include "MsgAlert.h"

const unsigned int ScreenWidth = 1120;
const unsigned int ScreenHeight = 800;
const int TileSize = 32;
const int GridWidth = 50;
const int GridHeight = 50;

class Game {
public:
	Game();

	void processInput();
	void update(const float dt=0.012);
	void draw();

	void run();
	void updateCulling();
public:
	void cameraBoundaries();
	void playerBoundaries();
public:
	void entityCollision();
	void tileCollision();
public:
	void msgUpdate();
	void resetFrameTime();
	void debugDisplay();
public:
	void drawString(const std::string& text,
					const sf::Vector2f position, 
					const unsigned int size,
					const sf::Color color = sf::Color::White,
					const sf::Color outline = sf::Color::Black);
private:
	sf::RenderWindow window;
	sf::View mainView;
	sf::View guiView;
private:
	sf::Clock dtClock;
	sf::Clock fpsClock;
	float dt;
	float frameTime;
	float fps;
	int currentFrames;
	bool showFps;
	float updateTimer;
private:
	float msgTimer;
private:
	Player player;
	Enemy enemy1;
	Grid grid;
	std::vector<Entity*> entities;
	sf::RectangleShape tileSelector;
private:
	bool ranIntoSolid;
	bool ranIntoEntity;
	bool showDebug;
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
};


