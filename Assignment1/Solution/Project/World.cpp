#include "World.hpp"

World::World(Game* game)
	: mSceneGraph(new SceneNode(game))
	, mGame(game)
	, mPlayerAircraft(nullptr)
	, mPlayer2Aircraft(nullptr)
	, mBackground(nullptr)
	, mWorldBounds(-1.5f, 1.5f, 200.0f, 0.0f) //Left, Right, Down, Up
	, mSpawnPosition(0.f, 0.f)
	, mScrollSpeed(1.0f)
{
}

void World::Update(const GameTimer& dt)
{
	ProcessInput();

	// Process and dispacth all commands
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph->OnCommand(mCommandQueue.pop(), dt);
	}

	mSceneGraph->Update(dt);
}

void World::Draw()
{
	mSceneGraph->Draw();
}

void World::BuildScene()
{
	/*--- Player Plane ---*/
	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mGame));
	mPlayerAircraft = player.get();
	mPlayerAircraft->SetPosition(-0.75, 1.5, 0.0);
	mPlayerAircraft->SetScale(0.0002, 0.0002, 0.0002);
	mPlayerAircraft->SetWorldRotation(XMConvertToRadians(270), XMConvertToRadians(270), 0.0f);
	mPlayerAircraft->SetVelocity(0.0, 0.0, 0.0);
	mSceneGraph->AttachChild(std::move(player));

	/*--- Enemy Plane ---*/
	std::unique_ptr<Aircraft> player2(new Aircraft(Aircraft::Raptor, mGame));
	mPlayer2Aircraft = player2.get();
	mPlayer2Aircraft->SetPosition(0.75, 1.5, 0.0);
	mPlayer2Aircraft->SetScale(0.0002, 0.0002, 0.0002);
	mPlayer2Aircraft->SetWorldRotation(XMConvertToRadians(270), XMConvertToRadians(270), 0.0f);
	mPlayer2Aircraft->SetVelocity(0, 0, 0);
	mSceneGraph->AttachChild(std::move(player2));
	
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(mGame));
	mBackground = backgroundSprite.get();
	//mBackground->SetPosition(mWorldBounds.Left, mWorldBounds.top);
	mBackground->SetPosition(0, 0, 0.0);
	mBackground->SetScale(10.0, 1.0, 200.0);
	mBackground->SetVelocity(0, 0, -mScrollSpeed);
	mSceneGraph->AttachChild(std::move(backgroundSprite));

	mSceneGraph->Build();
}

void World::ProcessInput()
{
	mPlayer.HandleRealtimeInput(GetCommandQueue());

	mPlayer2.HandleRealtimeInput(GetCommandQueue());
}

