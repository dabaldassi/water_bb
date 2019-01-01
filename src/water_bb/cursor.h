#ifndef CURSOR_H
#define CURSOR_H

#include <gameEngine/actor/controlable/controlable.h>

#include "parameters.h"

class Boat;

namespace actor {

  class Cursor : public Controlable, public Parameters<91, 135,0x0001,0x00002>
  {
    Boat * _boatColliding;
  public:

    Cursor();

    virtual void loadSprite();
    virtual void move(float dt = 0);
    virtual void act(float dt);
    virtual void collisionOn(Actor * a);

    void resetFilter();
    
    virtual ~Cursor() = default;
  };


}  // actor

#endif /* CURSOR_H */
