#include "Game.h"
#include <iostream>

Game::Game()
: window{sf::VideoMode{ScreenWidth,ScreenHeight},"SFML"}, grid{GridWidth,GridHeight,TileSize}
,fromX{0},toX{0},fromY{0},toY{0}
, showDebug{false}{
	this->window.setFramerateLimit(30);
	this->window.setVerticalSyncEnabled(false);
	this->dtClock.restart();
	this->frameTime = 0.f;

	this->font.loadFromFile("assets/fonts/sansation.ttf");
	this->debugBackdrop.setSize(sf::Vector2f{ScreenWidth/2.f,ScreenHeight/2.f});
	this->debugBackdrop.setFillColor(sf::Color{115,115,115,150});

	this->tileSelector.setSize(sf::Vector2f{(float)TileSize,(float)TileSize});
	this->tileSelector.setFillColor(sf::Color::Transparent);
	this->tileSelector.setOutlineThickness(1.f);
	this->tileSelector.setOutlineColor(sf::Color::Green);

	this->mainView.setSize(ScreenWidth, ScreenHeight);
	this->player.setPosition(14 * TileSize, 10 * TileSize);
	this->mainView.setCenter(this->player.getPosition());
	cameraBoundaries();
}

void Game::processInput() {
	sf::Event event;
	while (this->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			this->window.close();
		if(event.type == sf::Event::KeyPressed) {
			switch(event.key.code) {
				case sf::Keyboard::Tilde: this->showDebug = !this->showDebug;
			}
		}
	}

	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(this->window);
	mousePosView = static_cast<sf::Vector2i>(this->window.mapPixelToCoords(this->mousePosWindow));
	if(mousePosView.x >= 0.f)
			mousePosGrid.x = mousePosView.x / TileSize;
		if (mousePosView.y >= 0.f)
			mousePosGrid.y = mousePosView.y / TileSize;


	this->tileSelector.setPosition(mousePosGrid.x*TileSize,mousePosGrid.y*TileSize);


	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		this->grid.changeTile(mousePosView.x / TileSize,mousePosView.y/TileSize);
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

	playerBoundaries();

	this->mainView.setCenter(this->player.getPosition());
	cameraBoundaries();

	
}

void Game::update(float dt) {


	updateCulling();
}

void Game::draw() {
	this->window.clear();
	this->window.setView(this->mainView);
	// Draw Here

	this->grid.draw(&this->window,this->fromX,this->toX,this->fromY,this->toY);

	this->player.draw(&this->window);


	this->window.draw(this->tileSelector);


	this->window.setView(window.getDefaultView());

	if(this->showDebug)
		debugDisplay();
	// End Draw
	this->window.display();
}

void Game::run() {

	this->dt = this->dtClock.restart().asSeconds();
	while (window.isOpen()) {

		this->frameTime += this->dt;

		if (this->frameTime > 0.1f) {
			this->processInput();
		}
		this->update(dt);
		this->draw();
		this->dt = this->dtClock.restart().asSeconds();

	}
}

void Game::resetFrameTime() {
	this->frameTime = 0;
}

void Game::cameraBoundaries() {
	// The X Axis
	if (this->mainView.getCenter().x >= TileSize*GridWidth-(ScreenWidth/2.f))
		this->mainView.setCenter(TileSize*GridWidth-(ScreenWidth/2.f), this->mainView.getCenter().y);
	else if (this->mainView.getCenter().x <= ScreenWidth/2.f)
		this->mainView.setCenter(ScreenWidth/2.f, this->mainView.getCenter().y);
	// The Y Axis
	if (this->mainView.getCenter().y >= TileSize*GridHeight-(ScreenHeight/2.f))
		this->mainView.setCenter(this->mainView.getCenter().x, TileSize*GridHeight-(ScreenHeight/2.f));
	else if (this->mainView.getCenter().y <= ScreenHeight/2.f)
		this->mainView.setCenter(this->mainView.getCenter().x, ScreenHeight/2.f);


	std::cout<<mainView.getCenter().y<<"\n";
	std::cout<< ScreenHeight - (GridHeight*2)+TileSize<<"\n";
}

void Game::playerBoundaries() {
	// The X Axis
	if(player.getPosition().x <= 0)
		player.setPosition(0,player.getPosition().y);
	else if(player.getPosition().x >= GridWidth*TileSize-TileSize)
		player.setPosition(GridWidth*TileSize-TileSize,player.getPosition().y);

	// The Y Axis
	if(player.getPosition().y <= 0)
		player.setPosition(player.getPosition().x,0);
	else if(player.getPosition().y >= GridHeight*TileSize-TileSize)
		player.setPosition(player.getPosition().x,GridHeight*TileSize-TileSize);
}

void Game::debugDisplay() {
	this->window.draw(debugBackdrop);
	std::stringstream ss;
	ss << "Player Position: ("<<this->player.getPosition().x<<", "<<this->player.getPosition().y<<")\n"
	   << "Player Tile: ("<<this->player.getPosition().x/TileSize<<", "<<this->player.getPosition().y/TileSize<<")\n"
	   << "Screen: (" << mousePosScreen.x << ", " << mousePosScreen.y << ")\n"
	   << "Window: (" << mousePosWindow.x << ", " << mousePosWindow.y << ")\n"
	   << "View: (" << mousePosView.x << ", " << mousePosView.y << ")\n"
	   << "Grid: (" << mousePosGrid.x << ", " << mousePosGrid.y << ")\n";

	drawString(ss.str(),sf::Vector2f{125,75},18);
}

// Draw the given string at the given position.
void Game::drawString(std::string text, sf::Vector2f position, unsigned int size, sf::Color color, sf::Color outline) {
	// clear old data
	this->stringStream.str(std::string());
	this->stringStream.clear();

	this->stringStream << text;
	this->textString = this->stringStream.str();

	this->text.setFillColor(color);
	this->text.setOutlineColor(outline);
	this->text.setOutlineThickness(1.f);

	this->text.setString(this->textString);
	this->text.setFont(this->font);
	this->text.setCharacterSize(size);
	this->text.setPosition(position.x-(this->text.getLocalBounds().width / 2.f),position.y - (this->text.getLocalBounds().height / 2.f));

	this->window.draw(this->text);
}

void Game::updateCulling() {
	this->fromX = this->mainView.getCenter().x / TileSize - ((ScreenWidth/TileSize)/2)-1;
	this->toX = this->mainView.getCenter().x / TileSize + ((ScreenWidth/TileSize)/2)+1;

	this->fromY = this->mainView.getCenter().y / TileSize - ((ScreenHeight/TileSize)/2)-1;
	this->toY = this->mainView.getCenter().y / TileSize + ((ScreenHeight/TileSize)/2)+1;

	if(this->fromX < 0)
		this->fromX = 0;
	else if(this->fromX >= GridWidth*TileSize)
		this->fromX = GridWidth*TileSize - 1;

	if(this->toX < 0)
		this->toX = 0;
	else if(this->toX >= GridWidth*TileSize)
		this->toX = GridWidth*TileSize - 1;

	if(this->fromY < 0)
		this->fromY = 0;
	else if(this->fromY >= GridHeight*TileSize)
		this->fromY = GridHeight*TileSize - 1;

	if(this->toY < 0)
		this->toY = 0;
	else if(this->toY >= GridHeight*TileSize)
		this->toY = GridHeight*TileSize - 1;

	
}
