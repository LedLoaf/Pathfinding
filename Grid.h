#pragma once
#include "Tile.h"
#include <SFML/Graphics.hpp>

class Grid {
public:
	Grid(int width, int height, int tileWidth);

	void update();

	void draw(sf::RenderWindow* window, int fromX, int toX, int fromY, int toY);

	void changeTile(int x, int y);

private:
	std::vector<std::vector<Tile*>> tilemap;
	int gridWidth;
	int gridHeight;
	int tileWidth;
	sf::Texture tilesetTex;
	sf::Sprite tileset;
};
