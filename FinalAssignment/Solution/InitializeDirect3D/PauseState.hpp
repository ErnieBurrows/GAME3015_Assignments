#pragma once
#include "State.hpp"

class PauseState : public State
{
public:

    PauseState(StateStack* stack, Context* context);

    virtual ~PauseState();
    virtual void Draw() override;

    virtual bool Update(const GameTimer& gt) override;
    virtual bool HandleEvent(WPARAM btnState) override;
    virtual bool HandleRealTimeInput() override;

    bool showPause;
    void CheckPause(const GameTimer& gt);
};
