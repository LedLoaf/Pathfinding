#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
	Player();

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f pos);
	void setPosition(float x, float y);
	void move(float x, float y);

	void update();

	void draw(sf::RenderTarget* target);
private:
	sf::Texture texture;
	sf::Sprite sprite;
};
