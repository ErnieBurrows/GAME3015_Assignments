#include "StateStack.hpp"
#include <cassert>
#include "Game.hpp"

#pragma region step 6 - A3

StateStack::StateStack(State::Context context)
    : mStack()
    , mPendingList()
    , mContext(context)
    , mFactories()
{
}


void StateStack::Update(const GameTimer& timer)
{
   
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->Update(timer))
            break;
    }

    applyPendingChanges();
}

void StateStack::Draw()
{
    
    for (State::StatePtr& state : mStack)
    {
        state->Draw();
    }
}

void StateStack::HandleEvent(WPARAM btnState)
{
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->HandleEvent(btnState))
            break;
    }
}

void StateStack::handleRealTimeInput()
{
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->HandleRealTimeInput())
            break;
    }
}

void StateStack::pushState(States::ID stateID)
{
    mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
    mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
    mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
    return mStack.empty();
}

State* StateStack::GetCurrentState()
{
    return mStack.back().get();
}

State* StateStack::GetPreviousState()
{
    if (mStack.size() >= 2)
    {
        return mStack[mStack.size() - 2].get();
    }
    return nullptr;  // Explicit null return for safety
}

State::StatePtr StateStack::createState(States::ID stateID)
{
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end());

    return found->second();
}

void StateStack::applyPendingChanges()
{
    for (PendingChange change : mPendingList)
    {
        switch (change.action)
        {
        case Push:
            mStack.push_back(createState(change.stateID));
            break;
        case Pop:
            mStack.pop_back();
            break;
        case Clear:
            mStack.clear();
            break;
        }
    }

    mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
    : action(action)
    , stateID(stateID)
{
}

#pragma endregion