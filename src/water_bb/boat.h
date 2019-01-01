#ifndef BOAT_H
#define BOAT_H

#include <gameEngine/actor/moveable.hpp>
#include <gameEngine/actor/items/item.h>

#include "parameters.h"

b2Vec2 operator/(const b2Vec2 & vec, float div);

namespace actor {

  class Boat : public Moveable, public Parameters<91,51,0x0002>
  {
  protected:
    b2Vec2 _goal;
    bool   _isMoving;
    bool   _selected;
    float  _food;
    
  public:
    
    Boat(){}
    Boat(const std::string name, float life, const Position & p);

    virtual void move(float dt = 0);
    virtual void act(float dt) = 0;
    virtual void effect() = 0;
    virtual void effect(Actor * actor) = 0;
    
    virtual void setGoal(const b2Vec2 & vec) { _goal = vec, _isMoving = true; }
    virtual void select();
    virtual bool isSelected() const { return _selected; }
    virtual void display() const;
    virtual void clearDisplay() const {}
    
    virtual ~Boat() = default;
  };
}  // actor


#endif /* BOAT_H */
