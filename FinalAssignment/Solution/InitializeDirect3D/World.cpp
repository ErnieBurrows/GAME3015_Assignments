#define NOMINMAX
#include "World.hpp"


World::World(State* state)
	: mSceneGraph(new SceneNode(state))
	, mState(state)
	, mPlayerAircraft(nullptr)
	, mBackground(nullptr)
	, mWorldBounds(-3.25f, 3.25f, -1.5f, 2.5f) 
	, mSpawnPosition(0.f, 0.f)
	, mScrollSpeed(1.0f)
{
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}


void World::update(const GameTimer& gt)
{

	mPlayerAircraft->setVelocity(0.0f, 0.0f, 0.0f);
	while (!mCommandQueue.isEmpty())
		mSceneGraph->onCommand(mCommandQueue.pop(), gt);

	PlayerPosition();
	mSceneGraph->update(gt);

	float backgroundLength = 7.0f; // Match the Z-length of your background scale

	auto wrapBackground = [backgroundLength](SpriteNode* bg, SpriteNode* otherBg)
		{
			XMFLOAT3 pos = bg->getWorldPosition();
			if (pos.z <= -backgroundLength)
			{
				float newZ = otherBg->getWorldPosition().z + backgroundLength;
				bg->setPosition(pos.x, pos.y, newZ);
			}
		};

	wrapBackground(mBackground, mBackground2);
	wrapBackground(mBackground2, mBackground);

	PlayerVelocity();

}

void World::draw()
{
	mSceneGraph->draw();
}


void World::buildScene()
{
	std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mState));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(0, 0.1, 0.0);
	mPlayerAircraft->setScale(0.002, 0.002, 0.002);
	mPlayerAircraft->setWorldRotation(XMConvertToRadians(270), XMConvertToRadians(270), 0.0f);
	mPlayerAircraft->setVelocity(mScrollSpeed, 0.0, 0.0);
	mSceneGraph->attachChild(std::move(player));

	std::unique_ptr<SpriteNode> backgroundSprite1(new SpriteNode(mState));
	backgroundSprite1->SetDrawName("Desert", "boxGeo", "box");
	backgroundSprite1->setPosition(0, 0, 0.0f);
	backgroundSprite1->setScale(10.0f, 1.0f, 7.0f);
	backgroundSprite1->setVelocity(0, 0, -mScrollSpeed);
	mBackground = backgroundSprite1.get();
	mSceneGraph->attachChild(std::move(backgroundSprite1));

	std::unique_ptr<SpriteNode> backgroundSprite2(new SpriteNode(mState));
	backgroundSprite2->SetDrawName("Desert", "boxGeo", "box");
	backgroundSprite2->setPosition(0, 0, 7.0f); 
	backgroundSprite2->setScale(10.0f, 1.0f, 7.0f);
	backgroundSprite2->setVelocity(0, 0, -mScrollSpeed);
	mBackground2 = backgroundSprite2.get();
	mSceneGraph->attachChild(std::move(backgroundSprite2));

	std::unique_ptr<SpriteNode> PausePromptSprite(new SpriteNode(mState));
	PausePromptSprite->SetDrawName("PausePrompt", "boxGeo", "box");
	PausePromptSprite->setPosition(-1.75, 1.0, 1.75);
	PausePromptSprite->setScale(2.0, 0, 2.0);
	PausePromptSprite->setVelocity(0, 0, 0);
	mSceneGraph->attachChild(std::move(PausePromptSprite));


	mSceneGraph->build();
}

void World::PlayerPosition()
{
	const float borderDistance = 10.f;

	XMFLOAT3 position = mPlayerAircraft->getWorldPosition();
	position.x = std::max(position.x, mWorldBounds.x);
	position.x = std::min(position.x, mWorldBounds.y);
	position.z = std::max(position.z, mWorldBounds.z);
	position.z = std::min(position.z, mWorldBounds.w);
	mPlayerAircraft->setPosition(position.x, position.y, position.z);
}

void World::PlayerVelocity()
{
	XMFLOAT3 velocity = mPlayerAircraft->getVelocity();

	if (velocity.x != 0.0f && velocity.z != 0.0f)
	{
		mPlayerAircraft->setVelocity(velocity.x / std::sqrt(2.f), velocity.y / std::sqrt(2.f), velocity.z / std::sqrt(2.f));
	}
}
