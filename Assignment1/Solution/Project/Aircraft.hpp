#pragma once

#include "Entity.hpp"
#include "Game.hpp"

class Aircraft : public Entity
{
public:
    enum Type { Eagle, Raptor };

    Aircraft(Type type, Game* game);

    void DrawCurrent() const override;
    void BuildCurrent() override;

private:
    Type mType;
    std::string mModelName;
    std::string mMaterialName;

    RenderItem* renderer = nullptr;  
};
