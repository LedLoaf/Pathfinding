#include "Entity.h"

Entity::Entity(const std::string& spriteFile) {
	
	this->texture.loadFromFile(spriteFile,sf::IntRect());
	this->sprite.setTexture(this->texture, false);
}

void Entity::setScale(const float x, const float y) {this->sprite.setScale(x,y);}

sf::Vector2f Entity::getScale() {return this->sprite.getScale();}

void Entity::setTexture(const std::string& file) {this->texture.loadFromFile(file); this->sprite.setTexture(this->texture,false);}

void Entity::setSprite(sf::Texture* texture, const sf::IntRect rect) {this->sprite.setTexture(*texture), this->sprite.setTextureRect(rect);}

void Entity::setTextureRect(const sf::IntRect rect) {this->sprite.setTextureRect(rect);}

sf::Texture* Entity::getTexture() {	return &this->texture; }

sf::Sprite& Entity::getSprite() {return this->sprite;}

sf::FloatRect Entity::getGlobalBounds() const {return this->sprite.getGlobalBounds();}

sf::FloatRect Entity::getLocalBounds() const {return this->sprite.getLocalBounds();}

void Entity::setColor(const sf::Color color) {
	this->sprite.setColor(color);
}

sf::Vector2f Entity::getLastPosition() const {
	return this->lastPos;
}

void Entity::processInput() {
}

void Entity::update(const float dt) { auto temp = dt;	lastPos = getPosition(); }

void Entity::draw(sf::RenderTarget* target) {
	target->draw(this->sprite,sf::RenderStates::Default);
}

sf::Vector2f Entity::getPosition() {return this->sprite.getPosition();}

void Entity::setPosition(const sf::Vector2f pos) {this->sprite.setPosition(pos);}

void Entity::setPosition(const float x, const float y) {this->sprite.setPosition(x,y);}

void Entity::move(const float x, const float y) { this->sprite.move(x,y); }

void Entity::move(const sf::Vector2f pos) {	this->sprite.move(pos); }
