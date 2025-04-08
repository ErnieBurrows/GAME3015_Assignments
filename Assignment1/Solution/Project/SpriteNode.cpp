#include "SpriteNode.h"
#include "Game.hpp"

SpriteNode::SpriteNode(Game* game)
    : Entity(game), mMaterialName(""), mGeoName("")
{
}

void SpriteNode::SetMaterial(const std::string& materialName)
{
    mMaterialName = materialName;
}

void SpriteNode::SetGeometry(const std::string& geoName)
{
    mGeoName = geoName;
}

void SpriteNode::DrawCurrent() const
{
    if (renderer)
    {
        renderer->World = GetTransform();
        renderer->NumFramesDirty++;
    }
}

void SpriteNode::BuildCurrent()
{
    auto render = std::make_unique<RenderItem>();
    renderer = render.get();

    renderer->World = GetTransform();
    XMStoreFloat4x4(&renderer->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f)); // optional scaling

    renderer->ObjCBIndex = game->GetRenderItems().size();

    renderer->Mat = game->GetMaterials()[mMaterialName].get();
    renderer->Geo = game->GetGeometries()[mGeoName].get();
    renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    renderer->IndexCount = renderer->Geo->DrawArgs[mGeoName].IndexCount;
    renderer->StartIndexLocation = renderer->Geo->DrawArgs[mGeoName].StartIndexLocation;
    renderer->BaseVertexLocation = renderer->Geo->DrawArgs[mGeoName].BaseVertexLocation;

    game->GetRenderItems().push_back(std::move(render));
}
