#include <Box2D/Box2D.h>
#include <random>

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

  if(_life <= 0 && _currentSprite < 2) {
    setSpriteAnimationElement(_elem, 2);
    _currentSprite = 2;
    _life = 100;
    _food = 100;
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

void Warboat::wind()
{
  b2MassData mass;
  _body->GetMassData(&mass);
    
  _body->SetLinearVelocity(b2Vec2(WIDTH * 4, HEIGHT * 4));
  _body->ApplyLinearImpulse(b2Vec2(50, 50), mass.center, true);
}

void Warboat::thunder()
{
  kill();
}

// Kill by thunder
// Move by wind
// Nothing
// TP

void Warboat::effect()
{
  std::random_device rd{};
  std::mt19937       gen{rd()};
  std::normal_distribution<> d{0,4};
  void (Warboat::*effects[])() = {&Warboat::wind, &Warboat::thunder};

  int n = d(gen);

  if(n >= 0  && static_cast<unsigned int>(n) < sizeof(effects)/sizeof(void (Warboat::*)())) {
    (this->*effects[n])();
  }
}
