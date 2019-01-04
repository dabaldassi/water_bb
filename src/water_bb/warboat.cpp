#include "warboat.h"
#include "sprite.h"
#include "island.h"

using actor::Warboat;

Warboat::Warboat(const Position & p, bool team):Boat("warboat", LIFE, p, team)
{
  loadSprite();
  _flag = NULL;
}

void Warboat::act(float dt)
{
  Boat::act(dt);
  
  if(_isMoving) move();

  if(_life <= 0) {
    _flag->drop(_body->GetPosition());
    _flag = NULL;
  }

  int w,h;
  getDimensionWindow(&w, &h);

  if(_flag && ((_team && _body->GetPosition().x * Viewport::METER_TO_PIXEL < WIDTH) || (!_team && _body->GetPosition().x * Viewport::METER_TO_PIXEL > w - WIDTH - 10))) {
    _flag->effect();
  }
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
