#include "Grid.h"
#include <iostream>
#include "Tile.h"

Grid::Grid(int width, int height, int tileWidth) : gridWidth{width}, gridHeight{height}, tileWidth{tileWidth} {

	this->tilesetTex.loadFromFile("assets/tilemap_set.png");
	this->tileset.setTexture(tilesetTex);

	this->tilemap.resize(gridWidth* gridHeight);

	for (auto x = 0; x < gridWidth; x++) {
		this->tilemap.emplace_back(std::vector<Tile*>());
		for (auto y = 0; y < gridHeight; y++) {
			Tile* tile = new Tile{tileset};
			tile->color = tile->sprite.getColor();
			tile->sprite.setTextureRect(sf::IntRect{0,0,32,32});
			const auto ifRandomTree = rand() % 4;

			if (ifRandomTree == 2)
				tile->sprite.setTextureRect(sf::IntRect{64,0,32,32});

			if (x == 15 && y == 15) {
				tile->sprite.setColor(sf::Color::Red);
				tile->isSolid = true;
			}
			tile->position = sf::Vector2f{x * (float)tileWidth,y * (float)tileWidth};
			tile->sprite.setPosition(x * static_cast<float>(this->tileWidth), 
									 y * static_cast<float>(this->tileWidth));
			this->tilemap[x].push_back(tile);
		}
	}

	for (const auto& tiles : this->tilemap) {
		for (const auto& solidTiles : tiles) {
			if (solidTiles->isSolid)
				this->solids.push_back(solidTiles);
		}
	}
}

void Grid::update() {

	
}

void Grid::draw(sf::RenderWindow* window, const int fromX, const int toX, const int fromY, const int toY) {

	for (int x = fromX; x < toX; x++) {
		for (int y = fromY; y < toY; y++) {

			this->tilemap[x][y]->draw(window);
		}
	}

}

void Grid::changeTile(const int x, const int y, const sf::IntRect rect) {
	if(x > gridWidth || y > gridHeight)
		return;
	auto tile = this->tilemap[x][y];
	tile->setTextureRect(rect);
}

void Grid::changeTileColor(const int x, const int y, const sf::Color color) {
	if (x > gridWidth || y > gridHeight)
		return;
	auto tile = this->tilemap[x][y];
	tile->setColor(color);
}

void Grid::changeTileSprite(const int x, const int y, sf::Texture* texture) {
	auto tile = getTile(x,y);
	tile->setTexture(texture);
}

void Grid::setSolid(const int x, const int y, const bool isSolid) {
	const auto tile = getTile(x,y);
	tile->isSolid=isSolid;
}

bool Grid::isSolid(const int x, const int y) {
	const auto tile = getTile(x,y);
	return tile->isSolid;
}

Tile* Grid::getTile(const int x, const int y) {

	if (x > this->gridWidth || x < 0 || y > this->gridHeight || y < 0) {
		std::cout <<"Tile ("<< x << ", " << y << ")  is out of range\n";
		return nullptr;
	}

	return this->tilemap[x][y];
}
	
sf::Vector2f Grid::getSize() const {
	return sf::Vector2f(static_cast<float>(this->gridWidth),static_cast<float>(this->gridHeight));
}

void Grid::resetNodes() {
	for (int i = 0; i < gridWidth; i++)
	{
		for (int j = 0; j < gridHeight; j++)
		{
			Tile* cell = this->tilemap[i][j];
			cell->parentNode = nullptr;
			cell->H = 0;
			cell->G = 0;
			cell->F = 0;
		}
	}

}
