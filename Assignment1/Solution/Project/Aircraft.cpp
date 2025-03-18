#include "Aircraft.hpp"
#include "Game.hpp"

Aircraft::Aircraft(Type type, Game* game) : Entity(game)
	, mType(type)
{
	switch (type)
	{
	case (Eagle):
		mSprite = "Eagle";
		break;
	case (Raptor):
		mSprite = "Raptor";
		break;
	default:
		mSprite = "Eagle";
		break;
	}
}

void Aircraft::DrawCurrent() const
{	
	if (renderer) 
	{
		renderer->World = GetWorldTransform();
		renderer->NumFramesDirty++;
	}
}

void Aircraft::BuildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();

	renderer->World = GetTransform();
	renderer->ObjCBIndex = game->GetRenderItems().size();
	renderer->Mat = game->GetMaterials()[mSprite].get();
	renderer->Geo = game->GetGeometries()["planeGeo"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	renderer->IndexCount = renderer->Geo->DrawArgs["obj"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["obj"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["obj"].BaseVertexLocation;

	game->GetRenderItems().push_back(std::move(render));
}

unsigned int Aircraft::GetCategory() const
{
	// For now this assumes "Eagle is the player aircraft"
	return (mType == Eagle) ? Category::PlayerAircraft : Category::Player2Aircraft;
}

void Aircraft::Accelerate(XMFLOAT3& velocity)
{
	XMFLOAT3 currentVelocity = GetVelocity();

	currentVelocity.x += velocity.x;
	currentVelocity.y += velocity.y;
	currentVelocity.z += velocity.z;

	SetVelocity(currentVelocity);
}
