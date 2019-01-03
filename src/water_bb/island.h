#ifndef ISLAND_H
#define ISLAND_H

#include <gameEngine/actor/static.h>

#include "parameters.h"

namespace actor {

  class Island : public Static, public Parameters<91,102,0x0004>
  {
    float _food;
  
  public:
    static constexpr float FOOD = 800.f;
    
    Island(){}
    Island(const Position & p);

    virtual void act(float dt);

    float take();
 
    virtual ~Island() = default;
  };

}  // actor

#endif /* ISLAND_H */
