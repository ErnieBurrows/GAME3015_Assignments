#pragma once  
#include "State.h"  
#include <vector>  
#include <functional>  
#include <utility>  
#include <map>
#include <stdexcept>

class StateStack  
{  
public:  
   enum Action  
   {  
       Push,  
       Pop,  
       Clear  
   };  

   explicit StateStack(State::Context context) : mContext(context) {}  

   template<typename T>  
   void RegisterState(int stateID)  
   {  
       mFactories[stateID] = [this]()  
       {  
           return State::Ptr(new T(*this, mContext));  
       };  
   }  

   void Update(const GameTimer& gt);  
   void Draw();  
   void HandleEvent(WPARAM btnState, int x, int y);  
   void PushState(int stateID);  
   void PopState();  
   void ClearStates();  
   bool IsEmpty() const;  

   struct PendingChange
   {
       Action action;
       int stateID;
   };

private:  
    

   State::Ptr CreateState(int stateID);  
   void ApplyPendingChanges();  

   std::vector<State::Ptr> mStack;
   std::vector<PendingChange> mPendingList;
   State::Context mContext;
   std::map<int, std::function<State::Ptr()>> mFactories;
};
