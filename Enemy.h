#pragma once
#include "Entity.h"


class Grid;

const int tilesize = 32;

class Enemy final : public Entity {
public:
	explicit Enemy(const std::string& spriteFile);

	void update(float dt);
	void updatePathFinding(Grid& grid, const sf::Vector2f playerPos);
protected:
	// the target position of the enemy
	std::vector<sf::Vector2f> targetPositions;
};
