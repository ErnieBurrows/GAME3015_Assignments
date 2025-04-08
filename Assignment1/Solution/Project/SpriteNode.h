#pragma once
#include "Entity.hpp"
#include <string>

class SpriteNode : public Entity
{
public:
    explicit SpriteNode(Game* game);

    virtual void DrawCurrent() const override;
    virtual void BuildCurrent() override;

    void SetMaterial(const std::string& materialName);
    void SetGeometry(const std::string& geoName);

private:
    std::string mMaterialName;
    std::string mGeoName;
};
