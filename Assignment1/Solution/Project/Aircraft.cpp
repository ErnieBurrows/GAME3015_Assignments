#include "Aircraft.hpp"
#include "Game.hpp"

Aircraft::Aircraft(Type type, Game* game) : Entity(game), mType(type) 
{
    switch (type)
    {
    case Eagle:
        mModelName = "AircraftGeo";  // Matches stored geometry name
        mMaterialName = "Aircraft";  // Matches stored material name
        break;

    case Raptor:
        mModelName = "RaptorGeo";  // Ensure this geometry exists
        mMaterialName = "Raptor";  // Ensure this material exists
        break;

    default:
        mModelName = "AircraftGeo";
        mMaterialName = "Aircraft";
        break;
    }
}

void Aircraft::DrawCurrent() const
{
    // Entity should handle its own drawing if needed.
}

void Aircraft::BuildCurrent()
{
    auto render = std::make_unique<RenderItem>();
    renderer = render.get();

    renderer->World = GetTransform();
    renderer->ObjCBIndex = static_cast<UINT>(game->GetRenderItems().size()); 


    auto geometry = game->GetGeometries().find(mModelName);
    if (geometry == game->GetGeometries().end()) {
        throw std::runtime_error("Error: Geometry for '" + mModelName + "' not found.");
    }
    renderer->Geo = geometry->second.get();

    auto material = game->GetMaterials().find(mMaterialName);
    if (material == game->GetMaterials().end()) {
        throw std::runtime_error("Error: Material '" + mMaterialName + "' not found.");
    }
    renderer->Mat = material->second.get();

    XMMATRIX scaleMatrix = XMMatrixScaling(1.5f, 1.5f, 1.5f);
    XMMATRIX translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
    XMMATRIX worldMatrix = scaleMatrix * translationMatrix;

    XMStoreFloat4x4(&renderer->World, XMMatrixTranspose(worldMatrix));

    std::string drawArgKey = mModelName;
    if (renderer->Geo->DrawArgs.find(drawArgKey) == renderer->Geo->DrawArgs.end()) {
        throw std::runtime_error("Error: DrawArgs for '" + drawArgKey + "' not found.");
    }

    auto& drawArgs = renderer->Geo->DrawArgs[drawArgKey];
    renderer->IndexCount = drawArgs.IndexCount;
    renderer->StartIndexLocation = drawArgs.StartIndexLocation;
    renderer->BaseVertexLocation = drawArgs.BaseVertexLocation;

    renderer->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    game->GetRenderItems().push_back(std::move(render));
}
