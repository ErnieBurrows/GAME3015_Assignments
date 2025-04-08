#include "GameState.hpp"
#include "Game.hpp"
#include <windows.h>

GameState::GameState(StateStack* stack, Context* context)
	: State(stack, context)
	, mWorld(this)
	, mPauseStateSceneGraph(std::make_unique<SceneNode>(this))
{
	mAllRitems.clear();
	mContext->game->ResetFrameResources();
	mContext->game->BuildMaterials();

	mWorld.buildScene();

	std::unique_ptr<SpriteNode> PauseSprite = std::make_unique<SpriteNode>(this);
	PauseSprite->SetDrawName("PauseText", "boxGeo", "box");
	PauseSprite->setScale(3, 1, 3);
	PauseSprite->setPosition(0, 1, 0);
	mPauseStateSceneGraph->attachChild(std::move(PauseSprite));
	
	mPauseStateSceneGraph->build();

	mContext->game->BuildFrameResources(mAllRitems.size());
}

GameState::~GameState()
{
}

void GameState::Draw()
{
	mWorld.draw();
}

bool GameState::Update(const GameTimer& gt)
{
	mWorld.update(gt);
	return true;
}

bool GameState::HandleEvent(WPARAM btnState)
{
	if (d3dUtil::IsKeyDown('P'))
	{
		RequestStackPush(States::Pause);
	}

	return true;
}

bool GameState::HandleRealTimeInput()
{
	ProcessInput();

	return true;
}

void GameState::ProcessInput()
{
	CommandQueue& commands = mWorld.getCommandQueue();
	mContext->player->HandleEvent(commands);
	mContext->player->HandeRealTimeInput(commands);
}
