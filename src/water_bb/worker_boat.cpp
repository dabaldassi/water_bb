#include "worker_boat.h"
#include "sprite.h"

using actor::WorkerBoat;

WorkerBoat::WorkerBoat(const Position & p):Boat("worker",LIFE,p)
{
  /*this->addOnClickEvent( [](Actor * a) {
			  static_cast<WorkerBoat *>(a)->setMoveFlag();
			  
			}
			);*/

  loadSprite();
}

void WorkerBoat::act(float dt)
{
  if(_isMoving) move();
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
