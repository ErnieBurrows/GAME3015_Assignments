#pragma once
#include "Entity.hpp"
#include <string>

class Aircraft : public Entity
{
public:
	enum Type
	{
		Eagle,
		Raptor,
	};


public:
	Aircraft(Type type, Game* game);


private:
	virtual void DrawCurrent() const;
	virtual void BuildCurrent();


private:
	Type mType;
	std::string mSprite;
};
