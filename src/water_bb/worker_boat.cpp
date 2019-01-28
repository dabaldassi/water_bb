#include <Box2D/Box2D.h>
#include <random>

#include "worker_boat.h"
#include "warboat.h"
#include "sprite.h"
#include "island.h"
#include "sounds.h"

using actor::WorkerBoat;

#define NEXT(A) 

WorkerBoat::WorkerBoat(const Position & p, bool team):Boat("worker",LIFE,p,team)
{
  loadSprite();
  _foodCollected = 0;
  _foodCollectedBar = NULL;
  _capacity = 400.f;
  _isWorking = false;
  _island = NULL;
  
  addSound(RAVITAILLEMENT1);
  addSound(RAVITAILLEMENT2);
  addSound(RAVITAILLEMENT3);
  addSound(RAVITAILLEMENT4);
}

void WorkerBoat::act(float dt)
{
  Boat::act(dt);
  
  if(_isMoving) {
    move();
  }
    
  if(_isWorking) _foodCollected += _island->take();

  if(_isWorking && (_foodCollected >= _capacity || _island->empty())) {
    setDisplayCodeElement(_elem, 0, 1);
    setGoal(_lastPos);
  }

  if(_foodCollectedBar) *_foodCollectedBar = _foodCollected / _capacity;

  if(_food < 100 && _foodCollected > 0) refuel(&_foodCollected);
}

void WorkerBoat::collisionOn(Actor * actor)
{
  Island * island;
  Warboat * boat;
 
  if((island = dynamic_cast<Island *>(actor))) {
    _isWorking = true;
    _isMoving = false;
    _island = island;
    setDisplayCodeElement(_elem, 0, 0);
  }
  else if((boat = dynamic_cast<Warboat *>(actor))) {

    if(_team == boat->team()) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> d(0,3);
      std::string sounds[] = {RAVITAILLEMENT1,RAVITAILLEMENT2,RAVITAILLEMENT3,RAVITAILLEMENT4};

      int id = d(gen);
      
      boat->refuel(&_foodCollected);
      playSound(sounds[id]);
    }
    
    b2ContactEdge * edge = _body->GetContactList();
    
    while(edge) {
      edge->contact->SetEnabled(false);
      edge = edge->next;
    }

    setGoal(_lastPos);
  }
}

void WorkerBoat::collisionOff(Actor * actor)
{
}

void WorkerBoat::effect()
{
  
}

void WorkerBoat::effect(Actor * actor)
{
  
}

void WorkerBoat::loadSprite()
{
  Actor::loadSprite(BOAT_FOOD);
}

void WorkerBoat::setGoal(const b2Vec2 &vec)
{
  if(_isWorking) {
      _isWorking = false;
      _island = NULL;
      Boat::setGoal(vec);
  }
  else {
    Boat::setGoal(vec);
    _lastPos = _body->GetPosition();
  }
}

void WorkerBoat::display()
{
  int brown[] = {65,45,35,255};
  const b2Vec2 & p = getPosition<b2Vec2>();
  
  Boat::display();
  
  _foodCollectedBar = new ihm::ProgressBar(Position(p.x, p.y - Boat::WIDTH / 3 - 10, Boat::WIDTH - 10, 4), brown, _foodCollected / _capacity);
}

void WorkerBoat::clearDisplay()
{
  Boat::clearDisplay();

  delete _foodCollectedBar;

  _foodCollectedBar = NULL;
}

WorkerBoat::~WorkerBoat()
{
  if(_foodCollectedBar) delete _foodCollectedBar;
}
