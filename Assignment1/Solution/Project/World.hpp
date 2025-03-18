#pragma once
#include "SceneNode.hpp"
#include "Aircraft.hpp"
#include "SpriteNode.h"
#include "CommandQueue.h"

class World 
{
public:
	explicit World(Game* window);
	void Update(const GameTimer& gt);
	void Draw();

	//void LoadTextures();
	void BuildScene();

	void ProcessInput();


private:
	enum Layer
	{
		Background,
		Air,
		LayerCount
	};


private:
	Game* mGame;

	SceneNode* mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;

	XMFLOAT4 mWorldBounds;
	XMFLOAT2 mSpawnPosition;
	float mScrollSpeed;
	Aircraft* mPlayerAircraft;
	SpriteNode*	mBackground;
	Aircraft* mEnemy;

	/*--- Command Stuff ---*/
public:
	CommandQueue& GetCommandQueue() { return mCommandQueue; }

private:
	CommandQueue mCommandQueue;
};
