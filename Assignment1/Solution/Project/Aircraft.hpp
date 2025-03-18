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

	/*--- Command Stuff ---*/
public:
	virtual unsigned int GetCategory() const override;

	void StartBarrelRoll();

	virtual void UpdateCurrent(const GameTimer& gt) override;

private:
	bool mIsBarrelRolling = false;
	float mRollAccumulated = 0.0f;

};
