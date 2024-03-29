#ifndef WORKER_BOAT_H
#define WORKER_BOAT_H

#include "boat.h"

namespace actor {

  class Island;

  class WorkerBoat : public Boat
  {
  protected:
    bool     _isWorking;
    float    _foodCollected;
    Island * _island;
    float    _capacity;
    b2Vec2   _lastPos;

    ihm::ProgressBar * _foodCollectedBar;
    
  public:
    static constexpr float LIFE = 100.f;
    static constexpr float WIDTH = 53.f;
    static constexpr float HEIGHT = 22.f;
  
    WorkerBoat(){}
    WorkerBoat(const Position & p, bool team);

    virtual void act(float dt);
    virtual void effect();
    virtual void effect(Actor * actor);
    virtual void loadSprite();
    virtual void collisionOn(Actor * actor);
    virtual void collisionOff(Actor * actor);
    
    virtual void setGoal(const b2Vec2 & vec);
    virtual void display();
    virtual void clearDisplay();
    virtual bool deadboat() const { return _life <= 0; }
    
    virtual ~WorkerBoat();
  };


}  // actor

#endif /* WORKER_BOAT_H */
