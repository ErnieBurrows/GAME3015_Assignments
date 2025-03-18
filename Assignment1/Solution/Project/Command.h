#pragma once
#include <functional>
#include "../../Common/GameTimer.h"

class SceneNode;

struct Command
{
	std::function<void(SceneNode&, const GameTimer&)> action;
	unsigned int category;
};

