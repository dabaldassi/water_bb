#ifndef CURSOR_H
#define CURSOR_H

#include <gameEngine/actor/controlable/controlable.h>

class Boat;

namespace actor {

  class Cursor : public Controlable
  {
    Boat * _boatColliding;
  public:

    static constexpr float WIDTH = 91.42857142857;
    static constexpr float HEIGHT = 135;
    
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
