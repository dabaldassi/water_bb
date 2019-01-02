#include <gameEngine/game.h>
#include <gameEngine/actor/static.h>
#include <gameEngine/ihm/color.h>

#include "cursor.h"
#include "worker_boat.h"
#include "island.h"
#include "warboat.h"

void generate(Stage * stage)
{
  int w,h;

  getDimensionWindow(&w, &h);
  
  int NB_BLOCK_W = 21;
  int NB_BLOCK_H = 8;
  float SIZE_BLOCK_W = w/(float)NB_BLOCK_W;
  float SIZE_BLOCK_H = h/(float)NB_BLOCK_H;

  int sea_color[] = {38,120,155};

  createBlock(0, 0, w, h, sea_color, 0, 1);

  stage->create<actor::WorkerBoat>(Position(0,
					    4*SIZE_BLOCK_H + SIZE_BLOCK_H/2,
					    actor::Boat::WIDTH,
					    actor::Boat::HEIGHT),
				   true
				   );
  
  stage->create<actor::Cursor>(true);
  stage->create<actor::Cursor>(false);

  stage->create<actor::Island>(Position(0,0,actor::Island::WIDTH, actor::Island::HEIGHT));
  
  for(int j = 0; j < 2; j++)
    for(int i = 0; i < NB_BLOCK_H ; i++)
      stage->create<actor::Warboat>(Position(SIZE_BLOCK_W + j * (NB_BLOCK_W - 3) * SIZE_BLOCK_W,
					     i*SIZE_BLOCK_H + SIZE_BLOCK_H/2,
					     actor::Boat::WIDTH,
					     actor::Boat::HEIGHT),
				    j==0);
  
  
}

int main()
{
  Game game(1920,1080,"Water Boat Battle");
  
  game.run(generate);
  
  return 0;
}
