#include "Enemy.h"
#include "Grid.h"
#include <iostream>

Enemy::Enemy(const std::string& spriteFile) : Entity{spriteFile}{
	setTextureRect(sf::IntRect{32,128,16,16});
	setScale(2,2);

}
// Recalculates the enemies path finding.
void Enemy::update(float dt) {

	// Move towards current target location.
	if (!this->targetPositions.empty()) {
		sf::Vector2f targetLocation = this->targetPositions.front();
		auto m_velocity = sf::Vector2f(targetLocation.x - this->sprite.getPosition().x,
									   targetLocation.y - this->sprite.getPosition().y);

		this->sprite.setPosition(targetLocation);
		//if (abs(m_velocity.x) < 10.f && abs(m_velocity.y) < 10.f) {
		//	this->targetPositions.erase(this->targetPositions.begin());
		//}
	}
	//	else
	//	{
	//		float length = sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
	//	
	//		m_velocity.x /= length;
	//		m_velocity.y /= length;
	//		auto position = this->sprite.getPosition();
	//		position.x += m_velocity.x;
	//		position.y += m_velocity.y;

	//		this->sprite.setPosition(position);
	//	}
	//}

	// Call Entity update.
	Entity::update(dt);
}

void Enemy::updatePathFinding(Grid& grid, const sf::Vector2f playerPos) {
// Create all variables.
	std::vector<Tile*> openList;
	std::vector<Tile*> closedList;
	std::vector<Tile*> pathList;
	std::vector<Tile*>::iterator position;
	Tile* currentNode;

	// Reset all nodes.
	grid.resetNodes();

	// Store the start and goal nodes.
	Tile* startNode = grid.getTile(getPosition().x/tilesize,getPosition().y/tilesize);
	Tile* goalNode = grid.getTile(playerPos.x/tilesize,playerPos.y/tilesize);

	// Check we have a valid path to find. If not we can just end the function as there's no path to find.
	if (startNode == goalNode)
	{
		// Clear the vector of target positions.
		this->targetPositions.clear();

		// Exit the function.
		return;
	}

	// Pre-compute our H cost (estimated cost to goal) for each node.
	for (int i = 0; i < grid.getSize().x; i++)
	{
		for (int j = 0; j < grid.getSize().y; j++)
		{
			int rowOffset, heightOffset;
			Tile* node = grid.getTile(i, j);

			heightOffset = abs(node->position.x - goalNode->position.x);
			rowOffset = abs(node->position.y - goalNode->position.y);

			node->H = heightOffset + rowOffset;
		}
	}

	// Add the start node to the open list.
	openList.push_back(startNode);

	// While we have values to check in the open list.
	while (!openList.empty())
	{
		// Find the node in the open list with the lowest F value and mark it as current.
		int lowestF = INT_MAX;

		for (Tile* tile : openList)
		{
			if (tile->F < lowestF)
			{
				lowestF = tile->F;
				currentNode = tile;
			}
		}

		// Remove the current node from the open list and add it to the closed list.
		position = std::find(openList.begin(), openList.end(), currentNode);
		if (position != openList.end())
			openList.erase(position);

		closedList.push_back(currentNode);

		// Find all valid adjacent nodes.
		std::vector<Tile*> adjacentTiles;
		Tile* node;

		// Top.
		node = grid.getTile(currentNode->position.x/tilesize, (currentNode->position.y/tilesize) - 1);
		if ((node != nullptr) )
		{
			adjacentTiles.push_back(grid.getTile(currentNode->position.x/tilesize, (currentNode->position.y/tilesize) - 1));
		}

		// Right.
		node = grid.getTile((currentNode->position.x/tilesize) + 1, currentNode->position.y/tilesize);
		if ((node != nullptr) )
		{
			adjacentTiles.push_back(grid.getTile((currentNode->position.x/tilesize) + 1, currentNode->position.y/tilesize));
		}

		// Bottom.
		node = grid.getTile(currentNode->position.x/tilesize, (currentNode->position.y/tilesize) + 1);
		if ((node != nullptr))
		{
			adjacentTiles.push_back(grid.getTile(currentNode->position.x/tilesize, (currentNode->position.y/tilesize) + 1));
		}

		// Left.
		node = grid.getTile(((currentNode->position.x/tilesize) - 1), currentNode->position.y/tilesize);
		if ((node != nullptr) )
		{
			adjacentTiles.push_back(grid.getTile((currentNode->position.x/tilesize) - 1, currentNode->position.y/tilesize));
		}

		// For all adjacent nodes.
		for (Tile* node : adjacentTiles)
		{
			// If the node is our goal node.
			if (node == goalNode)
			{
				// Parent the goal node to current.
				node->parentNode = currentNode;

				// Store the current path.
				while (node->parentNode != nullptr)
				{
					pathList.push_back(node);
					node = node->parentNode;
				}

				// Empty the open list and break out of our for loop.
				openList.clear();
				break;
			}
			else
			{
				// If the node is not in the closed list.
				position = std::find(closedList.begin(), closedList.end(), node);
				if (position == closedList.end())
				{
					// If the node is not in the open list.
					position = std::find(openList.begin(), openList.end(), node);
					if (position == openList.end())
					{
						// Add the node to the open list.
						openList.push_back(node);

						// Set the parent of the node to the current node.
						node->parentNode = currentNode;

						// Calculate G (total movement cost so far) cost.
						node->G = currentNode->G + 10;

						// Calculate the F (total movement cost + heuristic) cost.
						node->F = node->G + node->H;
					}
					else
					{
						// Check if this path is quicker that the other.
						int tempG = currentNode->G + 10;

						// Check if tempG is faster than the other. I.e, whether it's faster to go A->C->B that A->C.
						if (tempG < node->G)
						{
							// Re-parent node to this one.
							node->parentNode = currentNode;
						}
					}
				}
			}
		}
	}

	// Clear the vector of target positions.
	this->targetPositions.clear();

	// Store the node locations as the enemies target locations.
	for (Tile* tile : pathList)
	{
		auto t = grid.getTile(tile->position.x / tilesize, tile->position.y / tilesize);
		this->targetPositions.push_back(t->getPosition());
	}

	// Reverse the target position as we read them from goal to origin and we need them the other way around.
	std::reverse(this->targetPositions.begin(), this->targetPositions.end());


}
