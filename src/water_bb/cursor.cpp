#include <gameEngine/ihm/keyboard.h>
#include <gameEngine/ihm/color.h>
#include <gameEngine/game.h>

#include <Box2D/Box2D.h>

#include "cursor.h"


using actor::Cursor;

Cursor::Cursor():Controlable("cursor", 1, Position(0,0,WIDTH,HEIGHT))
{
  b2Filter filter;

  filter.categoryBits = 1;
  filter.groupIndex = 2;
  filter.maskBits = (short)0;

  b2Fixture * fixture = _body->GetFixtureList();

  fixture->SetFilterData(filter);
  
  loadSprite();
}

void Cursor::move(float dt)
{
  b2Vec2 pos = _body->GetPosition();

  pos.x += (WIDTH / Viewport::METER_TO_PIXEL) * (ihm::Keyboard::keys[RIGHT] - ihm::Keyboard::keys[LEFT]);
  pos.y += (HEIGHT / Viewport::METER_TO_PIXEL) * (ihm::Keyboard::keys[FORWARD] - ihm::Keyboard::keys[BACK]);

  ihm::Keyboard::keys[RIGHT] = ihm::Keyboard::keys[LEFT] = ihm::Keyboard::keys[FORWARD] = ihm::Keyboard::keys[BACK] = false;
  
  _body->SetTransform(pos, _body->GetAngle());
}

void Cursor::act(float dt)
{
  move();
}

void Cursor::loadSprite()
{
  Actor::loadSprite(Color::yellow);

  setPlanElement(_elem, Game::GAME_D, 9);
}
