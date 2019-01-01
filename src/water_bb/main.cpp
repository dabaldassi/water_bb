#include <gameEngine/game.h>
#include <gameEngine/actor/static.h>
#include <gameEngine/ihm/color.h>

#include "cursor.h"
#include "worker_boat.h"
#include "island.h"

void generate(Stage * stage)
{
  int w,h;

  getDimensionWindow(&w, &h);
  
  int NB_BLOCK_W = 21;
  int NB_BLOCK_H = 8;
  float SIZE_BLOCK_W = w/NB_BLOCK_W;
  float SIZE_BLOCK_H = h/NB_BLOCK_H;

  int sea_color[] = {38,120,155};

  createBlock(0, 0, w, h, sea_color, 0, 10);

  stage->create<actor::WorkerBoat>(Position(0,4*SIZE_BLOCK_H + SIZE_BLOCK_H/2, actor::Boat::WIDTH, actor::Boat::HEIGHT));
  
  stage->create<actor::Cursor>();

  stage->create<actor::Island>(Position(0,0,actor::Island::WIDTH, actor::Island::HEIGHT));
}

int main()
{
  Game game(1920,1080,"Water Boat Battle");
  
  game.run(generate);
  
  return 0;
}
