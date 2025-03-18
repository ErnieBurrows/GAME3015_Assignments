#pragma once
#include <functional>
#include "SceneNode.hpp"
#include "../../Common/GameTimer.h"

struct Command
{
	std::function<void(SceneNode&, const GameTimer&)> action;
	unsigned int category;
};

