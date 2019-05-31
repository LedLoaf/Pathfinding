#include "Grid.h"

Grid::Grid(int width, int height, int tileWidth) : gridWidth{width},gridHeight{height}, tileWidth{tileWidth} {
	
	this->tilesetTex.loadFromFile("assets/tilemap_set.png");
	this->tileset.setTexture(tilesetTex);
	sf::IntRect tileRect;

	this->tilemap.resize(gridWidth*gridHeight);

	for(auto x = 0; x < gridWidth; x++) {
		this->tilemap.push_back(std::vector<Tile*>());
		for(auto y = 0; y < gridHeight; y++) {
			Tile* tile = new Tile{tileset};
			tile->color = tile->sprite.getColor();
			tile->sprite.setTextureRect(sf::IntRect{0,0,32,32});
			auto r = rand() % 4;

			if(r == 2)
			tile->sprite.setTextureRect(sf::IntRect{64,0,32,32});


			tile->sprite.setPosition(x*this->tileWidth,y*this->tileWidth);
			this->tilemap[x].push_back(tile);
		}
	}
}

void Grid::update() {


}

void Grid::draw(sf::RenderWindow* window, int fromX, int toX, int fromY, int toY) {
	
	for(int x = fromX; x < toX; x++) {
		for(int y = fromY; y < toY; y++) {
			
			this->tilemap[x][y]->draw(window);
		}
	}

}

void Grid::changeTile(int x, int y) {
	auto tile = this->tilemap[x][y];
	tile->colored = !tile->colored;

	if(tile->colored)
		tile->sprite.setColor(sf::Color::Green);
	else
		tile->sprite.setColor(tile->color);
}
