#include "worker_boat.h"
#include "sprite.h"
#include "island.h"

using actor::WorkerBoat;

WorkerBoat::WorkerBoat(const Position & p):Boat("worker",LIFE,p)
{
  loadSprite();
  _foodCollected = 0;
}

void WorkerBoat::act(float dt)
{
  if(_isMoving) {
    move();
  }
    
  if(_isWorking) _foodCollected += _island->take();
}

void WorkerBoat::collisionOn(Actor * actor)
{
  Island * island;
  
  if((island = dynamic_cast<Island *>(actor))) {
    _isWorking = true;
    _isMoving = false;
    _island = island;
    //_body->SetLinearVelocity(b2Vec2(0,0));
    //_body->SetAngularVelocity(0);
    //std::cout << "une ile !" << "\n";
  }
}

void WorkerBoat::collisionOff(Actor * actor)
{
  //std::cout << actor->getName() <<" OFF" << "\n";
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
  std::cout << "what ?" << "\n";
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

void WorkerBoat::display() const
{
  Boat::display();
  
  std::cout << "Food collected : " << _foodCollected << "\n";
}
