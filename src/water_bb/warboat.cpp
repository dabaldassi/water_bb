#include "warboat.h"
#include "sprite.h"
#include "island.h"

using actor::Warboat;

Warboat::Warboat(const Position & p, bool team):Boat("warboat", LIFE, p, team)
{
  loadSprite();
}

void Warboat::act(float dt)
{
  Boat::act(dt);
  
  if(_isMoving) move();
}

void Warboat::loadSprite()
{
  if(_team) {
    Actor::loadSprite(BOAT_WAR_2);
  }
  else {
    Actor::loadSprite(BOAT_WAR_1);
  }
}

void Warboat::collisionOn(actor::Actor *actor)
{
  if(dynamic_cast<Island *>(actor)) {
    _body->SetLinearVelocity(b2Vec2(0,0));
    _isMoving = false;
  }
}
