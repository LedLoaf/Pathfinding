#pragma once
#include <SFML/Graphics.hpp>


struct Tile {
public:
	sf::Sprite sprite;
	bool colored;
	sf::Color color;
	void setTexture(sf::Texture* tex) { sprite.setTexture(*tex);}
	sf::Vector2f getPosition() { return sprite.getPosition(); }

	void move(sf::Vector2f pos) {
		this->sprite.move(pos);
	}
	void move(float x, float y) {
		this->sprite.move(x,y);
	}

	void draw(sf::RenderTarget* target) {
		target->draw(this->sprite);
	}
};
