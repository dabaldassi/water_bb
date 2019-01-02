#include "warboat.h"
#include "sprite.h"

using actor::Warboat;

Warboat::Warboat(const Position & p, bool team):Boat("warboat", LIFE, p, team)
{
  loadSprite();
}

void Warboat::act(float dt)
{
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

