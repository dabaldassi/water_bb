#ifndef ISLAND_H
#define ISLAND_H

#include <gameEngine/actor/static.h>

#include "parameters.h"

namespace actor {

  class Island : public Static, public Parameters<275,135,0x0004>
  {
    float _food;
  
  public:
    static constexpr float FOOD = 1600.f;
    
    Island(){}
    Island(const Position & p);

    virtual void act(float dt);

    float take();
    bool empty() const { return _food <= 0; }
 
    virtual ~Island() = default;
  };

}  // actor

#endif /* ISLAND_H */
