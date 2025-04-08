#pragma once
#include "SceneNode.hpp"
#include "Aircraft.hpp"
#include "SpriteNode.h"

#include "CommandQueue.hpp"
#include "Command.hpp"

class World
{
public:

	explicit World(State* state);

	void update(const GameTimer& gt);
	void draw();
	void buildScene();

	CommandQueue& getCommandQueue();

private:
	CommandQueue mCommandQueue;

	void PlayerPosition();
	void PlayerVelocity();


private:
	enum Layer
	{
		Background,
		Air,
		LayerCount
	};


private:
	State* mState;

	SceneNode* mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;

	XMFLOAT4 mWorldBounds;
	XMFLOAT2 mSpawnPosition;
	float mScrollSpeed;
	Aircraft* mPlayerAircraft;
	SpriteNode* mBackground;
	SpriteNode* mBackground2;
	Aircraft* mEnemy;
};