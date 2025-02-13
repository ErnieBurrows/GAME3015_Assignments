#include "SpriteNode.h"
#include "Game.hpp"

SpriteNode::SpriteNode(Game* game) : Entity(game)
{

}

void SpriteNode::DrawCurrent() const
{
	renderer->World = GetTransform();
	renderer->NumFramesDirty++;
}

void SpriteNode::BuildCurrent()
{
	auto render = std::make_unique<RenderItem>();
	renderer = render.get();
	renderer->World = GetTransform();
	XMStoreFloat4x4(&renderer->TexTransform, XMMatrixScaling(10.0f, 10.0f, 10.0f));
	renderer->ObjCBIndex = game->GetRenderItems().size();
	renderer->Mat = game->GetMaterials()["Desert"].get();
	renderer->Geo = game->GetGeometries()["Aircraft"].get();
	renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	renderer->IndexCount = renderer->Geo->DrawArgs["Aircraft"].IndexCount;
	renderer->StartIndexLocation = renderer->Geo->DrawArgs["Aircraft"].StartIndexLocation;
	renderer->BaseVertexLocation = renderer->Geo->DrawArgs["Aircraft"].BaseVertexLocation;

	game->GetRenderItems().push_back(std::move(render));
}
