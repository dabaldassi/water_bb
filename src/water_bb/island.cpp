#include <gameEngine/ihm/color.h>
#include <gameEngine/game.h>
#include <Box2D/Box2D.h>
#include <math.h>

#include "island.h"
#include "boat.h"

using actor::Island;

Island::Island(const Position & p):Static("island", p)
{
  b2Filter filter;

  filter.categoryBits = CATEGORY;
  filter.maskBits = Boat::CATEGORY;
  _body->GetFixtureList()->SetFilterData(filter);

  Actor::loadSprite(Color::green);
  setPlanElement(_elem, Game::GAME_D, 9);
  
  _food = 200.f;
  
}


void Island::act(float dt)
{

}


float Island::take()
{
  float food = 0.f;
  
  if(_food > 0) {
    food = (0.5 > _food)?_food:0.5;
    _food -= food;

    if(_food <= 0) setColorElement(_elem, Color::red);
  }

  return food;
}
