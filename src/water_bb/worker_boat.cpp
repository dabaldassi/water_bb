#include "worker_boat.h"
#include "sprite.h"
#include "island.h"

using actor::WorkerBoat;

WorkerBoat::WorkerBoat(const Position & p, bool team):Boat("worker",LIFE,p,team)
{
  loadSprite();
  _foodCollected = 0;
  _foodCollectedBar = NULL;
  _capacity = 400.f;
}

void WorkerBoat::act(float dt)
{
  Boat::act(dt);
  
  if(_isMoving) {
    move();
  }
    
  if(_isWorking && _foodCollected < _capacity) _foodCollected += _island->take();

  if(_foodCollectedBar) *_foodCollectedBar = _foodCollected / _capacity;
}

void WorkerBoat::collisionOn(Actor * actor)
{
  Island * island;
  
  if((island = dynamic_cast<Island *>(actor))) {
    _isWorking = true;
    _isMoving = false;
    _island = island;
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
    if(vec != _island->body()->GetPosition()) {
      _isWorking = false;
      _island = NULL;
      Boat::setGoal(vec);
    }
  }
  else
    Boat::setGoal(vec);
}

void WorkerBoat::display()
{
  int brown[] = {65,45,35,255};
  const b2Vec2 & p = _body->GetPosition() * Viewport::METER_TO_PIXEL;
  
  Boat::display();
  
  _foodCollectedBar = new ihm::ProgressBar(Position(p.x, p.y - WIDTH / 3 - 10, WIDTH - 10, 4), brown, _foodCollected / _capacity);
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
