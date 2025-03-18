#include "World.hpp"

World::World(Game* game)
	: mSceneGraph(new SceneNode(game))
	, mGame(game)
	, mPlayerAircraft(nullptr)
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
	mPlayerAircraft->SetPosition(0, 1.5, 0.0);
	mPlayerAircraft->SetScale(0.0009, 0.0009, 0.0009);
	mPlayerAircraft->SetWorldRotation(XMConvertToRadians(270), XMConvertToRadians(270), 0.0f);
	mPlayerAircraft->SetVelocity(0.0, 0.0, 0.0);
	mSceneGraph->AttachChild(std::move(player));

	/*--- Enemy Plane ---*/
	std::unique_ptr<Aircraft> enemy1(new Aircraft(Aircraft::Raptor, mGame));
	auto raptor = enemy1.get();
	raptor->SetPosition(0, 1.5, 0.0);
	raptor->SetScale(0.0009, 0.0009, 0.0009);
	raptor->SetWorldRotation(XMConvertToRadians(270), XMConvertToRadians(270), 0.0f);
	raptor->SetVelocity(0, 0, 0);
	mSceneGraph->AttachChild(std::move(enemy1));
	
	//std::unique_ptr<Aircraft> enemy2(new Aircraft(Aircraft::Raptor, mGame));
	//auto raptor2 = enemy2.get();
	//raptor2->SetPosition(-0.5, 0, 1);
	//raptor2->SetScale(1.0, 1.0, 1.0);
	//raptor2->SetWorldRotation(0, XM_PI, 0);
	//mPlayerAircraft->AttachChild(std::move(enemy2));

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
}


/* OLD PLAYER MOVEMENT*/
/*
const float dt = gt.DeltaTime();
	const float moveSpeed = 5.0f * dt;
	const float rotationSpeed = 5.0f * XMConvertToRadians(45.0f) * dt; // Rotates at 45 degrees per second

	// Movement (Arrow Keys)
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		mPlayerAircraft->Move(0.0f, 0.0f, moveSpeed);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		mPlayerAircraft->Move(0.0f, 0.0f, -moveSpeed);
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		mPlayerAircraft->Move(-moveSpeed, 0.0f, 0.0f);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		mPlayerAircraft->Move(moveSpeed, 0.0f, 0.0f);
	}

	// Rotation (Q & R)
	if (GetAsyncKeyState('Q') & 0x8000) // Rotate counterclockwise (negative X-axis)
	{
		mPlayerAircraft->Rotate(rotationSpeed, 0.0f, 0.0f);
	}
	if (GetAsyncKeyState('E') & 0x8000) // Rotate clockwise (positive X-axis)
	{
		mPlayerAircraft->Rotate(-rotationSpeed, 0.0f, 0.0f);
	}
*/


