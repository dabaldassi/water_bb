#ifndef BOAT_H
#define BOAT_H

#include <gameEngine/actor/moveable.hpp>
#include <gameEngine/actor/items/item.h>
#include <gameEngine/ihm/progressBar.h>

#include "parameters.h"

b2Vec2 operator/(const b2Vec2 & vec, float div);
b2Vec2 operator*(const b2Vec2 & vec, float coeff);
bool operator!=(const b2Vec2 & vec1, const b2Vec2 & vec2);

namespace actor {

  class Boat : public Moveable, public Parameters<91,51,0x0002>
  {
  protected:
    b2Vec2 _goal;
    bool   _isMoving;
    bool   _selected;
    float  _food;
    bool   _team;
    int    _timeFood;
    float  _timerFood;
    
    ihm::ProgressBar * _lifeBar;
    ihm::ProgressBar * _foodBar;
    
  public:
    
    Boat(){}
    Boat(const std::string name, float life, const Position & p, bool team);
    
    virtual void move(float dt = 0);
    virtual void act(float dt);
    virtual void effect() = 0;
    virtual void effect(Actor * actor) = 0;
    
    virtual void setGoal(const b2Vec2 & vec) { _goal = vec, _isMoving = true; }
    virtual void select();
    virtual bool isSelected() const { return _selected; }
    virtual void display();
    virtual void clearDisplay();
    virtual bool team() const { return _team; }
    virtual void refuel(float * food);
    
    virtual ~Boat();
  };
}  // actor


#endif /* BOAT_H */
