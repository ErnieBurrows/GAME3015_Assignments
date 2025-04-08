#pragma region Step 5
#pragma once
#include "Category.hpp"
#include "../../Common/d3dApp.h"
#include <functional>
#include <cassert>

class SceneNode;

struct Command
{
  
	Command();
	std::function<void(SceneNode&, const GameTimer&)> action;

	unsigned int category;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, const GameTimer&)> derivedAction(Function fn)
{
    return [=](SceneNode& node, const GameTimer& gameTimer)
        {
            assert(dynamic_cast<GameObject*>(&node) != nullptr);

            fn(static_cast<GameObject&>(node), gameTimer);
        };
}
#pragma endregion