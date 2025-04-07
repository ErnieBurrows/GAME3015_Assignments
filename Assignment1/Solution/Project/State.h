#pragma once

#include "../../Common/d3dApp.h"
#include "FrameResource.h"
#include "SceneNode.hpp"
#include <memory>
#include<Windows.h>

namespace States
{
	/**
	 * @brief Enumeration of game state types
	 */
	enum ID
	{
		None,           ///< Null/invalid state
		Title,          ///< Title screen state
		Menu,           ///< Main menu state
		Game,           ///< Core gameplay state
		Instructions,   ///< Tutorial/instructions state
		Pause           ///< Paused game state
	};
}

class StateStack;
class Game;
class Player;
class SceneNode;

namespace sf
{
	class RenderWindow;
}

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
	virtual bool HandleEvent(WPARAM btnState) = 0;
	virtual bool HandleRealTimeInput() = 0;

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

