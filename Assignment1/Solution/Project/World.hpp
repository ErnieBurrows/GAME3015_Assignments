#pragma once
#include "SceneNode.hpp"
#include "Aircraft.hpp"
#include "SpriteNode.h"
#include "CommandQueue.h"
#include "Player.h"
#include "Player2.h"

class World 
{
public:
	explicit World(Game* window);
	void Update(const GameTimer& gt);
	void Draw();

	//void LoadTextures();
	void BuildScene();

	void ProcessInput();

	size_t GetNumRenderItems() const;


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
	Aircraft* mPlayer2Aircraft;

	SpriteNode*	mBackground;
	Aircraft* mEnemy;

	/*--- Command Stuff ---*/
public:
	CommandQueue& GetCommandQueue() { return mCommandQueue; }

	void CollectRenderItems(std::vector<RenderItem*>& renderItems) const;

private:
	CommandQueue mCommandQueue;

	Player mPlayer;
	Player2 mPlayer2;
};
