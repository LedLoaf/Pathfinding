#include "Player.h"

Player::Player() {
	
	this->texture.loadFromFile("media/characters.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setTextureRect(sf::IntRect{0,0,16,16});
	this->sprite.setScale(2,2);
}

sf::Vector2f Player::getPosition() {return this->sprite.getPosition();}

void Player::setPosition(sf::Vector2f pos) {this->sprite.setPosition(pos);}

void Player::setPosition(float x, float y) {this->sprite.setPosition(x,y);}

void Player::move(float x, float y) {
	this->sprite.move(x,y);
}

void Player::update() {
}

void Player::draw(sf::RenderTarget* target) {
	target->draw(this->sprite);
}
