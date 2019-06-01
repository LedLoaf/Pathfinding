#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
	explicit Entity(const std::string& spriteFile);
	Entity() = default;
	virtual ~Entity() = default;

	void move(float x, float y);
	void move(const sf::Vector2f pos);
	void setPosition(const sf::Vector2f pos);
	void setPosition(const float x, const float y);
	sf::Vector2f getPosition();
	sf::Vector2f getLastPosition() const;

	void setSprite(sf::Texture* texture, const sf::IntRect rect = sf::IntRect{0,0,32,32});
	sf::Sprite& getSprite();
	void setTextureRect(const sf::IntRect rect);

	void setTexture(const std::string& file);
	sf::Texture* getTexture();

	sf::FloatRect getLocalBounds() const;
	sf::FloatRect getGlobalBounds() const;

	void setColor(const sf::Color color);

	void setScale(const float x, const float y);
	sf::Vector2f getScale();

	virtual void processInput();

	virtual void update(const float dt);

	virtual void draw(sf::RenderTarget* target);
protected:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::IntRect rect;
	sf::Vector2f lastPos;
};
