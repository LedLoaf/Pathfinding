#include "Game.h"
#include <iostream>

Game::Game()
	: window{sf::VideoMode{ScreenWidth, ScreenHeight, 32},"Rogue Example",sf::Style::Default,sf::ContextSettings()}
	, dt{0.f}, fps{0.f}, currentFrames{0}, showFps{false}, msgTimer{0.f}
	, player{"media/characters.png"}
	, enemy1{"media/characters.png"}
	, grid{GridWidth, GridHeight, TileSize}
	, ranIntoSolid{false}, ranIntoEntity{false}, showDebug{false}
	, fromX{0}, toX{0}
	, fromY{0}, toY{0} 
{
	this->window.setFramerateLimit(30);
	this->window.setVerticalSyncEnabled(false);
	this->dtClock.restart();
	this->frameTime = 0.f;

	this->font.loadFromFile("assets/fonts/sansation.ttf");
	this->debugBackdrop.setSize(sf::Vector2f{ScreenWidth / 2.f,ScreenHeight / 2.f});
	this->debugBackdrop.setFillColor(sf::Color{115,115,115,150});

	this->tileSelector.setSize(sf::Vector2f{static_cast<float>(TileSize),static_cast<float>(TileSize)});
	this->tileSelector.setFillColor(sf::Color::Transparent);
	this->tileSelector.setOutlineThickness(1.f);
	this->tileSelector.setOutlineColor(sf::Color::Green);

	this->mainView.setSize(ScreenWidth, ScreenHeight);

	this->player.setPosition(14 * TileSize, 10 * TileSize);
	this->enemy1.setPosition(18 * TileSize, 12 * TileSize);
	this->entities.push_back(&player);
	this->entities.push_back(&enemy1);

	this->mainView.setCenter(this->player.getPosition());
	cameraBoundaries();
}

void Game::processInput() {
	sf::Event event{};
	while (this->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			this->window.close();
		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
				case sf::Keyboard::Escape:
					this->window.close();
					break;
				case sf::Keyboard::Tilde:
					if (!event.key.control) this->showDebug = !this->showDebug;
					else this->showFps = !this->showFps;
					break;
			}
		}
	}

	this->window.setView(this->mainView);

	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(this->window);
	this->mousePosView = static_cast<sf::Vector2i>(this->window.mapPixelToCoords(this->mousePosWindow));
	if (this->mousePosView.x >= 0.f)
		this->mousePosGrid.x = this->mousePosView.x / TileSize;
	if (this->mousePosView.y >= 0.f)
		this->mousePosGrid.y = this->mousePosView.y / TileSize;

	this->tileSelector.setPosition(static_cast<float>(this->mousePosGrid.x) * TileSize,
								   static_cast<float>(this->mousePosGrid.y) * TileSize);


	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		this->grid.changeTileColor(this->mousePosView.x / TileSize,
								   this->mousePosView.y / TileSize,
								   sf::Color{255,255,255,255});
		const auto tile = this->grid.getTile(this->mousePosView.x / TileSize,
									   this->mousePosView.y / TileSize);
		tile->isSolid = false;
		resetFrameTime();
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		this->grid.changeTileColor(this->mousePosView.x / TileSize,
								   this->mousePosView.y / TileSize,
								   sf::Color::Red);
		const auto tile = this->grid.getTile(this->mousePosView.x / TileSize,
									   this->mousePosView.y / TileSize);
		tile->isSolid = true;
		resetFrameTime();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->player.move(0, -TileSize);
		resetFrameTime();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->player.move(-TileSize, 0);
		resetFrameTime();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->player.move(0, TileSize);
		resetFrameTime();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->player.move(TileSize, 0);
		resetFrameTime();
	}



	entityCollision();
	tileCollision();
	playerBoundaries();

	this->mainView.setCenter(this->player.getPosition());
	cameraBoundaries();


}

void Game::update(const float dt) {

	updateTimer += dt;

	if (updateTimer > 1.2f) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			enemy1.updatePathFinding(this->grid, this->player.getPosition());

			for (auto& entity : this->entities) {
				entity->update(dt);
			}

			updateTimer = 0;
		}
	}
	updateCulling();
}

void Game::draw() {
	this->window.clear(sf::Color::Black);
	this->window.setView(this->mainView);
	// Draw Here

	this->grid.draw(&this->window, this->fromX, this->toX, this->fromY, this->toY);

	for (auto& entity : this->entities) {
		entity->draw(&this->window);
	}
	msgUpdate();


	this->window.draw(this->tileSelector, sf::RenderStates::Default);


	this->window.setView(this->window.getDefaultView());

	if (showFps)
		drawString(std::string("FPS: " + std::to_string(static_cast<int>(this->fps))), sf::Vector2f{ScreenWidth - 32,9}, 12);
	if (this->showDebug)
		debugDisplay();

	// End Draw
	this->window.display();
}

void Game::run() {
	fpsClock.restart();
	while (this->window.isOpen()) {

		this->fps = static_cast<float>(currentFrames) / fpsClock.getElapsedTime().asSeconds();
		this->dt = this->dtClock.restart().asSeconds();

		this->frameTime += this->dt;

		if (this->frameTime > 0.1f) {
			processInput();
		}
		update(dt);
		draw();
		++currentFrames;

	}
}

void Game::msgUpdate() {


	if (this->ranIntoSolid) {
		drawString("Tile is Solid",
				   sf::Vector2f{this->player.getPosition().x + (sizeof("Tile is Solid") / 2) + 12,
					this->player.getPosition().y - 12},
				   12, sf::Color::Red);

		this->msgTimer += this->dt;

		if (this->msgTimer >= 1.2f) {
			this->msgTimer = 0;
			this->ranIntoSolid = !this->ranIntoSolid;
		}
	}

	if (this->ranIntoEntity) {
		if (this->ranIntoSolid)
			drawString("Enemy is Solid",
					   sf::Vector2f{this->player.getPosition().x + (sizeof("Enemy is Solid") / 2) + 12,
						this->player.getPosition().y - 24},
					   12, sf::Color::Red);
		else
			drawString("Enemy is Solid",
					   sf::Vector2f{this->player.getPosition().x + (sizeof("Enemy is Solid") / 2) + 12,
						this->player.getPosition().y - 12},
					   12, sf::Color::Red);

		this->msgTimer += this->dt;

		if (this->msgTimer >= 1.2f) {
			this->msgTimer = 0;
			this->ranIntoEntity = !this->ranIntoEntity;
		}
	}
}

void Game::resetFrameTime() {
	this->frameTime = 0;
}

void Game::cameraBoundaries() {
	// The X Axis
	if (this->mainView.getCenter().x >= TileSize * GridWidth - (ScreenWidth / 2.f))
		this->mainView.setCenter(TileSize * GridWidth - (ScreenWidth / 2.f), this->mainView.getCenter().y);
	else if (this->mainView.getCenter().x <= ScreenWidth / 2.f)
		this->mainView.setCenter(ScreenWidth / 2.f, this->mainView.getCenter().y);
	// The Y Axis
	if (this->mainView.getCenter().y >= TileSize * GridHeight - (ScreenHeight / 2.f))
		this->mainView.setCenter(this->mainView.getCenter().x, TileSize * GridHeight - (ScreenHeight / 2.f));
	else if (this->mainView.getCenter().y <= ScreenHeight / 2.f)
		this->mainView.setCenter(this->mainView.getCenter().x, ScreenHeight / 2.f);
}

void Game::playerBoundaries() {
	// The X Axis
	if (this->player.getPosition().x <= 0)
		this->player.setPosition(1, this->player.getPosition().y);
	else if (this->player.getPosition().x >= GridWidth * TileSize - TileSize)
		this->player.setPosition(GridWidth * TileSize - TileSize-1, this->player.getPosition().y);

	// The Y Axis
	if (this->player.getPosition().y <= 0)
		this->player.setPosition(this->player.getPosition().x, 1);
	else if (this->player.getPosition().y >= GridHeight * TileSize - TileSize)
		this->player.setPosition(this->player.getPosition().x, GridHeight * TileSize - TileSize-1);
}

void Game::entityCollision() {

	for (auto& entity : this->entities) {
		for (auto& e : this->entities) {
			if (entity == e)
				continue;
			if (entity->getGlobalBounds().intersects(e->getSprite().getGlobalBounds())) {
				entity->setPosition(entity->getLastPosition());
				this->ranIntoEntity = true;
			}
		}
	}
}

void Game::tileCollision() {
	const auto x = static_cast<int>(this->player.getPosition().x) / TileSize;
	const auto y = static_cast<int>(this->player.getPosition().y) / TileSize;

	const auto tile = this->grid.getTile(x, y);
	if (!tile)
		std::cout << "Tile (" << x << ", " << y << ") does not exist. (nullptr)\n";
	else {
		if (tile->isSolid) {
			if (this->player.getGlobalBounds().intersects(tile->sprite.getGlobalBounds())) {
				this->player.setPosition(this->player.getLastPosition());
			}
			this->ranIntoSolid = true;
		}
	}
}

void Game::debugDisplay() {
	// Displays some data for the debug box
	this->window.draw(this->debugBackdrop);

	std::stringstream ss;
	ss << "Display Resolution: " << ScreenWidth << "x" << ScreenHeight << "\n"
		<< "TileSize: " << TileSize << "    MapWidth: " << GridWidth << "    MapHeight: " << GridHeight << "    Total Tiles: " << GridWidth * GridHeight << "\n"
		<< "Frames Per Second: " << static_cast<int>(this->fps) << "\n"
		<< "Clock Time: " << this->dtClock.getElapsedTime().asSeconds() << "\n"
		<< "Delta Time: " << this->dt << "\n"
		<< "Player Position: (" << this->player.getPosition().x << ", " << this->player.getPosition().y << ")\n"
		<< "Player Tile: (" << static_cast<int>(this->player.getPosition().x) / TileSize << ", " << static_cast<int>(this->player.getPosition().y) / TileSize << ")\n"
		<< "Mouse Screen Relativity: (" << this->mousePosScreen.x << ", " << this->mousePosScreen.y << ")\n"
		<< "Mouse Window Relativity: (" << this->mousePosWindow.x << ", " << this->mousePosWindow.y << ")\n"
		<< "Mouse View Relativity: (" << this->mousePosView.x << ", " << this->mousePosView.y << ")\n"
		<< "Mouse Grid Relativity: (" << this->mousePosGrid.x << ", " << this->mousePosGrid.y << ")\n"
		<< "Enemy1 Position: (" << this->enemy1.getPosition().x << ", " << this->enemy1.getPosition().y << ")\n"
		<< "Enemy1 Tile: (" << this->enemy1.getPosition().x / TileSize << ", " << this->enemy1.getPosition().y / TileSize << ")\n"
		<< "Culling: From:(" << this->fromX << ", " << this->fromY << ") To: (" << this->toX << ", " << this->toY << ")\n";

	drawString(ss.str(), sf::Vector2f{255,155}, 18);
}

// Draw the given string at the given position.
void Game::drawString(const std::string& text, 
					  const sf::Vector2f position, 
					  const unsigned int size, 
					  const sf::Color color, const sf::Color outline) {
	// clear old data
	this->stringStream.str(std::string());
	this->stringStream.clear(std::ios::goodbit,false);

	this->stringStream << text;
	this->textString = this->stringStream.str();

	this->text.setFillColor(color);
	this->text.setOutlineColor(outline);
	this->text.setOutlineThickness(1.f);

	this->text.setString(this->textString);
	this->text.setFont(this->font);
	this->text.setCharacterSize(size);
	this->text.setPosition(position.x - (this->text.getLocalBounds().width / 2.f), position.y - (this->text.getLocalBounds().height / 2.f));

	this->window.draw(this->text,sf::RenderStates::Default);
}

void Game::updateCulling() {
	this->fromX = static_cast<int>(this->mainView.getCenter().x) / TileSize - (static_cast<int>(ScreenWidth / TileSize) / 2) - 1;
	this->toX   = static_cast<int>(this->mainView.getCenter().x) / TileSize + (static_cast<int>(ScreenWidth / TileSize) / 2) + 1;

	this->fromY = static_cast<int>(this->mainView.getCenter().y) / TileSize - (static_cast<int>(ScreenHeight / TileSize) / 2) - 1;
	this->toY   = static_cast<int>(this->mainView.getCenter().y) / TileSize + (static_cast<int>(ScreenHeight / TileSize) / 2) + 1;

	if (this->fromX < 0)
		this->fromX = 0;
	else if (this->fromX >= GridWidth * TileSize)
		this->fromX = GridWidth * TileSize - 1;

	if (this->toX < 0)
		this->toX = 0;
	else if (this->toX >= GridWidth * TileSize)
		this->toX = GridWidth * TileSize - 1;

	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY >= GridHeight * TileSize)
		this->fromY = GridHeight * TileSize - 1;

	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY >= GridHeight * TileSize)
		this->toY = GridHeight * TileSize - 1;


}
