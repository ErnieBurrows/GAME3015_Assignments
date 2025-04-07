#pragma once

#include "../../Common/d3dApp.h"
#include "FrameResource.h"
#include "SceneNode.hpp"
#include <memory>
#include<Windows.h>

class StateStack;
class Game;
class Player;
class SceneNode;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	// Context struct to pass information and pointers to each state
	struct Context
	{

		Context(Game* _game, Player* _player);
		Game* game;
		Player* player;
	};

	State(StateStack& stack, Context context);

	virtual ~State();

	virtual void Draw() = 0;
	virtual bool Update(const GameTimer& gt) = 0;
	virtual bool HandleEvent(WPARAM btnState, int x, int y) = 0;

	virtual bool IsTransparent() const { return false; }

	std::vector<std::unique_ptr<RenderItem>>& getRenderItems() { return mAllRitems; }


protected:
	void RequestStackPush(int stateID);
	void RequestStackPop();
	void RequestStackClear();

	
	Context GetContext() const { return mContext; }

private:
	StateStack* mStack;
	Context mContext;

	std::unique_ptr<SceneNode> mSceneGraph; 
	std::vector<std::unique_ptr<RenderItem>> mAllRitems; 
};

