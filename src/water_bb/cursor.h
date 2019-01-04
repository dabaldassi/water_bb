#ifndef CURSOR_H
#define CURSOR_H

#include <gameEngine/actor/controlable/controlable.h>

#include "parameters.h"

namespace actor {

  class Boat;

  class Cursor : public Controlable, public Parameters<91, 135,0x0001>
  {
    Boat * _boatColliding;
    bool   _team;
    int    _turnLeft;

    static bool _turn;
    
  public:
    static constexpr float WIDTH = WIDTH + 0.42857;
    static constexpr int   NB_TURN = 30;
    
    Cursor();
    Cursor(bool team);

    virtual void loadSprite();
    virtual void move(float dt = 0);
    virtual void act(float dt);
    virtual void effect();
    virtual void collisionOn(Actor * a);
    
    void nextTurn();
    void resetFilter();
    bool checkMove();
    
    virtual ~Cursor() = default;
  };


}  // actor

#endif /* CURSOR_H */
