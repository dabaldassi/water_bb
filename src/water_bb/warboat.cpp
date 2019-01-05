#include "warboat.h"
#include "sprite.h"
#include "island.h"

using actor::Warboat;

Warboat::Warboat(const Position & p, bool team):Boat("warboat", LIFE, p, team)
{
  loadSprite();
  _flag = NULL;
  _currentSprite = 0;
}

void Warboat::act(float dt)
{
  if(!deadboat())
    Boat::act(dt);
  else {
    nextSpriteElement(_elem);
    if(++_currentSprite == NB_SPRITES) kill();
  }
  
  if(_life <= 0 && _currentSprite < 2) {
    setSpriteAnimationElement(_elem, 2);
    _currentSprite = 2;
    _life = 100;
    _food = 100;
  }

  if(_flag && _life <= 0) {
    _flag->drop(_body->GetPosition());
    _flag = NULL;
  }
  
  if(_isMoving && !deadboat()) move();
  
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

  addAnimationElement(_elem, 0);

  for(int i =0; i < 10;i++)
    addSpriteAnimationElement(_elem, 0, i*WIDTH*2, 0, WIDTH*2, HEIGHT*2, 10, i);
  
  setWaySpriteAnimationElement(_elem, 0, 0);
}

void Warboat::collisionOn(actor::Actor *actor)
{
  if(dynamic_cast<Island *>(actor)) {
    _body->SetLinearVelocity(b2Vec2(0,0));
    _isMoving = false;
  }
}
