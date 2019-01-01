#include <gameEngine/ihm/keyboard.h>
#include <gameEngine/ihm/color.h>
#include <gameEngine/game.h>

#include <Box2D/Box2D.h>

#include "boat.h"
#include "cursor.h"


using actor::Cursor;

Cursor::Cursor():Controlable("cursor", 1, Position(0,0,WIDTH,HEIGHT))
{
  _boatColliding = NULL;
  
  resetFilter();

  loadSprite();
}

void Cursor::collisionOn(Actor * a)
{
  b2Filter f;
  f.categoryBits = 0x0001;
  f.maskBits = 0x0000;
  _body->GetFixtureList()->SetFilterData(f);
  
  b2ContactEdge * c = _body->GetContactList();
      
  while(c) {
    c->contact->SetEnabled(false);
    c = c->next;
  }

  _boatColliding = static_cast<Boat *>(a);
  
}

void Cursor::resetFilter()
{
  b2Filter filter; // Filter not to collide with anything

  filter.categoryBits = CATEGORY;
  filter.maskBits = Boat::CATEGORY;

  b2Fixture * fixture = _body->GetFixtureList();

  fixture->SetFilterData(filter);

  if(_boatColliding && !_boatColliding->isSelected())
    _boatColliding = NULL;
}

void Cursor::move(float dt)
{
  b2Vec2 pos = _body->GetPosition();

  pos.x += (WIDTH / Viewport::METER_TO_PIXEL) * (ihm::Keyboard::keys[RIGHT] - ihm::Keyboard::keys[LEFT]);
  pos.y += (HEIGHT / Viewport::METER_TO_PIXEL) * (ihm::Keyboard::keys[FORWARD] - ihm::Keyboard::keys[BACK]);

  ihm::Keyboard::keys[RIGHT] = ihm::Keyboard::keys[LEFT] = ihm::Keyboard::keys[FORWARD] = ihm::Keyboard::keys[BACK] = false;

  if(_boatColliding && (pos.x != _body->GetPosition().x || pos.y != _body->GetPosition().y))
    resetFilter(); // Reset the filter when the cursor moves and there is currently a collision with a boat
  
  _body->SetTransform(pos, _body->GetAngle());
  
}

void Cursor::act(float dt)
{
  move();

  if(ihm::Keyboard::keys[INTERACT] && _boatColliding) {
    std::cout << "ok" << "\n";
    
    if(_boatColliding->isSelected())
      _boatColliding->setGoal(_body->GetPosition());
    
    _boatColliding->select();

    ihm::Keyboard::keys[INTERACT] = false;
  }
}

void Cursor::loadSprite()
{
  Actor::loadSprite(Color::yellow);

  setPlanElement(_elem, Game::GAME_D, 9);
}
