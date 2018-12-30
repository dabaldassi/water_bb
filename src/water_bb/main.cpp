#include <gameEngine/game.h>
#include <gameEngine/actor/static.h>

#include "worker_boat.h"

void generate(Stage * stage)
{
  int w,h;

  getDimensionWindow(&w, &h);
  
  int NB_BLOCK_W = 21;
  int NB_BLOCK_H = 8;
  float SIZE_BLOCK_W = w/NB_BLOCK_W;
  float SIZE_BLOCK_H = h/NB_BLOCK_H;

  int sea_color[] = {20,50,200};

  createBlock(0, 0, w, h, sea_color, 0, 10);

  actor::WorkerBoat * boat = &stage->create<actor::WorkerBoat>(Position(0,4*SIZE_BLOCK_H + SIZE_BLOCK_H/2, actor::Boat::WIDTH, actor::Boat::HEIGHT));

  boat->setGoal(b2Vec2(w/4,h/4));
  
}

int main()
{
  Game game(1920,1080,"Water Boat Battle");
  
  game.run(generate);
  
  return 0;
}
