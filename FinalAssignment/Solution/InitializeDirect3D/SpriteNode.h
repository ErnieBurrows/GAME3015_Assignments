#pragma once
#include "Entity.hpp"


class SpriteNode : public Entity
{
public:

	SpriteNode(State* state);


	RenderItem* mSpriteNodeRitem;

	void SetDrawName(std::string Mat, std::string Geo, std::string DrawName);
	std::string GetDrawName();
	void SetVisible(bool visible);
private:
	
	virtual void drawCurrent() const;
	
	virtual void buildCurrent();

	std::string mMat;
	std::string mGeo;
	std::string mDrawName;
	bool mIsVisible;
};
