#pragma once
#include "Entity.h"


class Player final : public Entity {
public:
	explicit Player(const std::string& spriteFile);
};
