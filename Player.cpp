#include "Player.h"

Player::Player(const std::string& spriteFile) : Entity{spriteFile}
{
	setTextureRect(sf::IntRect{0,0,16,16});
	setScale(2,2);
}
