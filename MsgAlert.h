#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using MessageContainer = std::vector<std::string>;

class MsgAlert{
public:
	MsgAlert();
	MsgAlert(int visible, int charSize, int width, sf::Vector2f screenPos);
	~MsgAlert();

	void setup(int visible, int charSize, int width, sf::Vector2f screenPos, bool noBackDrop = true);
	void add(std::string message);
	void updatePosition(sf::Vector2f pos, float dt);
	void clear();

	void render(sf::RenderWindow& wind);
private:
	MessageContainer messages;
	int numVisible;

	bool noBackDrop;
	float timer;

	sf::RectangleShape backdrop;
	sf::Font font;
	sf::Text content;
};