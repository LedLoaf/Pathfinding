#include "MsgAlert.h"
#include <iostream>

MsgAlert::MsgAlert(){
	setup(5,9,200,sf::Vector2f(0,0));
	timer = 0;
}

MsgAlert::MsgAlert(int visible, int charSize, int width, sf::Vector2f screenPos){
	setup(visible,charSize, width, screenPos);
	timer = 0;
}

MsgAlert::~MsgAlert(){ clear(); }

void MsgAlert::setup(int visible, int charSize, int width, sf::Vector2f screenPos, bool noBackDrop){
	this->numVisible = visible;
	this->noBackDrop = noBackDrop;
	sf::Vector2f offset(2.0f, 2.0f);
	
	this->font.loadFromFile("assets/fonts/sansation.ttf");
	this->content.setFont(this->font);
	this->content.setString("");
	this->content.setCharacterSize(charSize);
	this->content.setFillColor(sf::Color::White);
	this->content.setPosition(screenPos + offset);

	this->backdrop.setSize(sf::Vector2f(width, ((float)visible * ((float)charSize * 1.2f))));
	this->backdrop.setFillColor(sf::Color(90,90,90,90));
	this->backdrop.setPosition(screenPos);
}

void MsgAlert::add(std::string message){
	this->messages.push_back(message);
	if (this->messages.size() < 6){ return; }
	this->messages.erase(this->messages.begin());
}

void MsgAlert::updatePosition(sf::Vector2f pos, float dt) {
	this->content.setPosition(pos);
	timer+=dt;

	if(timer > 1.5f && this->messages.size() > 0){
		while(this->messages.begin() != this->messages.end())
			this->messages.erase(this->messages.begin());
		timer = 0;
	}
}

void MsgAlert::clear(){ this->messages.clear(); }

void MsgAlert::render(sf::RenderWindow& wind){
	std::string content;

	for(auto &itr : this->messages){
		content.append(itr+"\n");
	}

	if(content != ""){
		this->content.setString(content);
		if(!noBackDrop)
			wind.draw(this->backdrop);
		wind.draw(this->content);
	}
}