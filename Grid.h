#pragma once
#include "Tile.h"
#include <SFML/Graphics.hpp>

class Grid {
public:
	Grid(int width, int height, int tileWidth);

	void update();

	void draw(sf::RenderWindow* window, const int fromX, const int toX, const int fromY, const int toY);

	void changeTile(const int x, const int y, const sf::IntRect rect);
	void changeTileColor(const int x, const int y, const sf::Color color);
	void changeTileSprite(const int x, const int y, sf::Texture* texture);
	void setSolid(const int x, const int y, const bool isSolid);
	bool isSolid(const int x, const int y);
	

	Tile* getTile(const int x, const int y);

	sf::Vector2f getSize() const;

	void resetNodes();

private:
	std::vector<std::vector<Tile*>> tilemap;
	std::vector<Tile*> solids;
	int gridWidth;
	int gridHeight;
	int tileWidth;
	sf::Texture tilesetTex;
	sf::Sprite tileset;
};
