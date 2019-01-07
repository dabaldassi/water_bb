#ifndef WARBOAT_H
#define WARBOAT_H

#include "boat.h"

namespace actor {

  class Weapon;
  
  class Warboat : public Boat
  {
  protected:
    Item * _flag;
    Weapon * _canon;
    int    _currentSprite;
    b2Vec2  _goalCanon;
    
  public:
    static constexpr float LIFE = 100.f;
    static constexpr int   NB_SPRITES = 10;
    static unsigned int nb_boat_1;
    static unsigned int nb_boat_2;
    
    Warboat(){}
    Warboat(const Position & p, bool team);
  
    virtual void act(float dt);
    virtual void effect();
    virtual void effect(Actor * actor){}
    virtual void loadSprite();
    virtual void collisionOn(Actor * actor);
    virtual void attack(const b2Vec2 & pos);
    virtual const b2Vec2 & getGoalCanon() const { return _goalCanon; }

    virtual void addItem(Item * item);
    virtual void removeItem(Item *) { _flag = NULL; }
    
    virtual bool deadboat() const { return _currentSprite >= 2; }

    void wind();
    void thunder();
    
    virtual ~Warboat() = default;
  };

}  // actor

#endif /* WARBOAT_H */
