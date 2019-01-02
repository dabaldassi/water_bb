#ifndef WARBOAT_H
#define WARBOAT_H

#include "boat.h"

namespace actor {

  class Warboat : public Boat
  {
  protected:
    
  public:
    static constexpr float LIFE = 100.f;
    
    Warboat(){}
    Warboat(const Position & p, bool team);
  
    virtual void act(float dt);
    virtual void effect(){}
    virtual void effect(Actor * actor){}
    virtual void loadSprite();
  
    virtual ~Warboat() = default;
  };

}  // actor




#endif /* WARBOAT_H */
