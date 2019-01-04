#include <gameEngine/ihm/keyboard.h>
#include <gameEngine/ihm/color.h>
#include <gameEngine/game.h>
#include <math.h>

#include <Box2D/Box2D.h>

#include "worker_boat.h"
#include "cursor.h"

using actor::Cursor;

bool Cursor::_turn = false;

Cursor::Cursor(bool team):Controlable("cursor", 1, Position(4*WIDTH + !team*(2*WIDTH),0,WIDTH,HEIGHT))
{
  _boatColliding = NULL;
  _team = team;
  
  resetFilter();

  loadSprite();

  if(_team == _turn) _turnLeft = NB_TURN;
}

void Cursor::collisionOn(Actor * a)
{
  b2Filter f;
  f.categoryBits = 0x0001;
  f.maskBits = 0x0000; // Not collide when touch something
  _body->GetFixtureList()->SetFilterData(f);
  
  b2ContactEdge * c = _body->GetContactList();
      
  while(c) {
    c->contact->SetEnabled(false); //Disable all contact when the cursor collides

    /* Sometimes cursor is FixtureA and sometimes Fixrure B ... */
    
    void * dataA = c->contact->GetFixtureA()->GetBody()->GetUserData();
    void * dataB = c->contact->GetFixtureB()->GetBody()->GetUserData();
    
    Actor * actorA = static_cast<Actor *>(dataA);
    Actor * actorB = static_cast<Actor *>(dataB);
    
    Actor * actor = (actorA == this)?actorB:actorA; // Take the actor which is not the cursor

    // Get the body the nearest from the cursor
    if(actor && abs(actor->body()->GetPosition().x - _body->GetPosition().x) < abs(a->body()->GetPosition().x - _body->GetPosition().x)) {
      a = actor;
    }
    
    c = c->next;
  }

  // Affect the boat colliding if this actor is a baot and if there is no boat selected
  if((!_boatColliding || !_boatColliding->isSelected())) {
    Boat * boat = dynamic_cast<Boat *>(a);

    //Check if the boat is in the smae case as the cursor and if this is the same team
    if(boat && roundf(_body->GetPosition().x) == roundf(boat->body()->GetPosition().x) && _team == boat->team()) {
      _boatColliding = boat;
    }
  }
  
}

void Cursor::resetFilter()
{
  b2Filter filter; // Filter to collide with boat

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

bool Cursor::checkMove()
{
  return (dynamic_cast<WorkerBoat *>(_boatColliding) ||
	  (_team == _turn &&
	   (((int)round(abs(_boatColliding->body()->GetPosition().x - _body->GetPosition().x)
		  * Viewport::METER_TO_PIXEL / WIDTH))  |
	   ((int)round(abs(_boatColliding->body()->GetPosition().y - _body->GetPosition().y)
		       * Viewport::METER_TO_PIXEL / HEIGHT))) == 1));
}

void Cursor::effect()
{
  if(_boatColliding->isSelected() && checkMove()) {
    _boatColliding->setGoal(_body->GetPosition());

    if(!dynamic_cast<WorkerBoat *>(_boatColliding))
      _turnLeft--;
  }
    
  _boatColliding->select();
}

void Cursor::act(float dt)
{
  if(_boatColliding && _boatColliding->getLife() <= 0)
    _boatColliding = NULL;
  
  move();

  if(_team && ihm::Keyboard::keys[INTERACT] && _boatColliding) {
    effect();
    
    ihm::Keyboard::keys[INTERACT] = false;
  }

  if(!_team && ihm::Keyboard::keys[JUMP] && _boatColliding) {
    effect();
    ihm::Keyboard::keys[JUMP] = false;
  }

  if(_turnLeft == 0) {
    _turn = !_turn;
    _turnLeft = NB_TURN;
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
