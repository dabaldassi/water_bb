#include <gameEngine/ihm/color.h>
#include <gameEngine/game.h>
#include <Box2D/Box2D.h>

#include "island.h"
#include "boat.h"
#include "sprite.h"

using actor::Island;

Island::Island(const Position & p):Static("island", p)
{
  b2Filter filter;

  filter.categoryBits = CATEGORY;
  filter.maskBits = Boat::CATEGORY;
  _body->GetFixtureList()->SetFilterData(filter);

  Actor::loadSprite(ISLAND_1);
  
  _food = FOOD;
  
}


void Island::act(float dt)
{
  if(_food <= 2*FOOD/3.f && _food > FOOD/3.f) setImageElement(_elem, ISLAND_2);
  if(_food <= 0) setImageElement(_elem, ISLAND_3);
}


float Island::take()
{
  float food = 0.f;
  
  if(_food > 0.f) {
    food = (0.5 > _food)?_food:0.5;
    _food -= food;
  }

  return food;
}
