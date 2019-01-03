#include <gameEngine/ihm/keyboard.h>
#include <gameEngine/ihm/color.h>
#include <gameEngine/game.h>
#include <math.h>

#include <Box2D/Box2D.h>

#include "boat.h"
#include "cursor.h"


using actor::Cursor;

Cursor::Cursor(bool team):Controlable("cursor", 1, Position(4*WIDTH + !team*(2*WIDTH),0,WIDTH,HEIGHT))
{
  _boatColliding = NULL;
  _team = team;
  
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
  
  if((!_boatColliding || !_boatColliding->isSelected())) {
    Boat * boat = dynamic_cast<Boat *>(a);
    
    if(roundf(_body->GetPosition().x) == roundf(boat->body()->GetPosition().x) && boat && _team == boat->team()) {
      _boatColliding = boat;
    }
  }
  
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

  pos.x += (WIDTH / Viewport::METER_TO_PIXEL) *
    (((_team && ihm::Keyboard::keys[RIGHT]) || (!_team && ihm::Keyboard::keys[RIGHT_2])) -
     ((_team && ihm::Keyboard::keys[LEFT]) || (!_team && ihm::Keyboard::keys[LEFT_2])));
  
  pos.y += (HEIGHT / Viewport::METER_TO_PIXEL) *
    (((_team && ihm::Keyboard::keys[FORWARD]) || (!_team && ihm::Keyboard::keys[FORWARD_2])) -
     ((_team && ihm::Keyboard::keys[BACK]) || (!_team && ihm::Keyboard::keys[BACK_2])));

  if(!_team)
    ihm::Keyboard::keys[RIGHT_2] = ihm::Keyboard::keys[LEFT_2] = ihm::Keyboard::keys[FORWARD_2] = ihm::Keyboard::keys[BACK_2] = false;
  else
    ihm::Keyboard::keys[RIGHT] = ihm::Keyboard::keys[LEFT] = ihm::Keyboard::keys[FORWARD] = ihm::Keyboard::keys[BACK] = false;
  
  if((pos.x != _body->GetPosition().x || pos.y != _body->GetPosition().y))
    resetFilter(); // Reset the filter when the cursor moves and there is currently a collision with a boat
  
  _body->SetTransform(pos, _body->GetAngle());
}

void Cursor::effect()
{
  std::cout << _boatColliding->getName() << "\n";
    
  if(_boatColliding->isSelected())
    _boatColliding->setGoal(_body->GetPosition());
    
  _boatColliding->select();
}

void Cursor::act(float dt)
{
  // b2Vec2 v = _body->GetLinearVelocity();
  
  // if(v.x != 0 || v.y != 0) _body->SetLinearVelocity(b2Vec2(0,0));
  
  move();

  if(_team && ihm::Keyboard::keys[INTERACT] && _boatColliding) {
    effect();
    
    ihm::Keyboard::keys[INTERACT] = false;
  }

  if(!_team && ihm::Keyboard::keys[JUMP] && _boatColliding) {
    effect();
    ihm::Keyboard::keys[JUMP] = false;
  }
}

void Cursor::loadSprite()
{
  int color1[] = {255,255,0,80};
  int color2[] = {255,255,255,80};
  
  if(_team)
    setColorElement(_elem, color1);
  else
    setColorElement(_elem, color2);
    
  setPlanElement(_elem, Game::GAME_D, 1);
}
