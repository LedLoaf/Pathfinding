#pragma once
#include <SFML/Graphics.hpp>


struct Tile {
public:
	sf::Sprite sprite;					// The tile's sprite
	int H;								// Heuristic / movement cost to goal.
	int G;								// Movement cost. (Total of entire path)
	int F;								// Estimated cost for full path. (G + H)
	Tile* parentNode = nullptr;					// Node to reach this node.

	sf::Vector2f position;

	bool isSolid;						// Tile is not able to be walked on
	sf::Color color;					// Tile sprite color of the tile
public:
	void setTexture(sf::Texture* tex) { this->sprite.setTexture(*tex,false);}
	void setTextureRect(const sf::IntRect rect) {this->sprite.setTextureRect(rect);}
	
	sf::Vector2f getPosition() { return this->sprite.getPosition(); }
	void setPosition(const sf::Vector2f pos) {this->sprite.setPosition(pos);}
	void setPosition(const float x, const float y){this->sprite.setPosition(x,y);}

	void setColor(const sf::Color color){this->sprite.setColor(color);}
	
	void move(const sf::Vector2f pos) {
		this->sprite.move(pos);
	}
	void move(const float x, const float y) {
		this->sprite.move(x,y);
	}

	void draw(sf::RenderTarget* target) const {
		target->draw(this->sprite, sf::RenderStates::Default);
	}
};
