#ifndef WORKER_BOAT_H
#define WORKER_BOAT_H

#include "boat.h"

namespace actor {

  class WorkerBoat : public Boat
  {
  protected:
    bool _isWorking;
  public:
    static constexpr float LIFE = 100.f;
    
    WorkerBoat(){}
    WorkerBoat(const Position & p);

    virtual void act(float dt);
    virtual void effect();
    virtual void effect(Actor * actor);
    virtual void loadSprite();
    
    virtual ~WorkerBoat() = default;
  };


}  // actor

#endif /* WORKER_BOAT_H */
