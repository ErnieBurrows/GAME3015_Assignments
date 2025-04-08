#pragma once

#include "State.h"
#include "FrameResource.h"
#include "RenderItem.h"
#include <vector>
#include <memory>

class TitleState : public State
{
public:
    TitleState(StateStack& stack, Context context);

    virtual void Draw();
    virtual bool Update(const GameTimer& gt);
    virtual bool HandleEvent(WPARAM btnState, int x, int y);

private:
    void BuildFrameResources();

private:
    float mElapsedTime;

    std::vector<std::unique_ptr<RenderItem>> mRenderItems;
    std::vector<std::unique_ptr<FrameResource>> mFrameResources;
    int mCurrFrameResourceIndex;
    FrameResource* mCurrFrameResource;

    PassConstants mMainPassCB;
};
