#pragma once

#include "State.h"
#include "World.hpp"
#include "FrameResource.h"
#include <memory>
#include <vector>

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);

	virtual ~GameState();

	virtual void Draw();
	virtual bool Update(float dt);
	virtual bool HandleEvent(WPARAM btnState, int x, int y);
	
private:
	void BuildFrameResources();

	std::unique_ptr<World> mWorld;
	std::vector<std::unique_ptr<FrameResource>> mFrameResources;
	int mCurrFrameResourceIndex;
	FrameResource* mCurrFrameResource;

};