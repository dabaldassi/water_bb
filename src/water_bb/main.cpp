#include <gameEngine/game.h>
#include <gameEngine/actor/static.h>
#include <gameEngine/ihm/color.h>
#include <gameEngine/actor/items/item.h>

#include "cursor.h"
#include "worker_boat.h"
#include "island.h"
#include "warboat.h"
#include "sprite.h"

void collisionRabbit(actor::Actor * actor, actor::Actor * b)
{
  actor::Item    * rabbit = static_cast<actor::Item *>(actor);
  actor::Warboat * boat = dynamic_cast<actor::Warboat *>(b);

  if(rabbit->isOnTheGround() && boat) {
    boat->pickFlag(rabbit->pick());
  }
}

void effectRabbit(actor::Actor * actor)
{
  actor::Item * rabbit = static_cast<actor::Item *>(actor);

  rabbit->stage()->endStage();

  std::cout << "win" << "\n";
}

void generate(Stage * stage)
{
  int w,h;

  getDimensionWindow(&w, &h);
  
  int NB_BLOCK_W = 21;
  int NB_BLOCK_H = 8;
  float SIZE_BLOCK_W = w/(float)NB_BLOCK_W;
  float SIZE_BLOCK_H = h/(float)NB_BLOCK_H;

  int sea_color[] = {38,120,155,255};

  SDL_SetRenderTarget(_windows_SANDAL2->current->renderer, NULL);
  SDL_SetRenderDrawBlendMode(_windows_SANDAL2->current->renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(_windows_SANDAL2->current->renderer, 128, 128, 128, 255);

  createBlock(0, 0, w, h, sea_color, 0, 5);

  actor::Item * rabbit = &stage->create<actor::Item>("rabbit",
						    Position(SIZE_BLOCK_W * (NB_BLOCK_W>>1) +
							     (SIZE_BLOCK_W - 85) / 2.f,
							     SIZE_BLOCK_H * (NB_BLOCK_H>>1) +
							     (SIZE_BLOCK_H - 85) / 2.f,
							     85,
							     85),
						     12.f);
  rabbit->loadSprite(RABBIT);
  rabbit->addCollisionOnStatement(collisionRabbit);
  rabbit->addEffectStatement(effectRabbit);
  
  stage->create<actor::WorkerBoat>(Position(SIZE_BLOCK_W / 2.f - actor::WorkerBoat::WIDTH / 2.f,
					    4*SIZE_BLOCK_H +
					    (SIZE_BLOCK_H/2.f - actor::WorkerBoat::HEIGHT / 2.f),
					    actor::WorkerBoat::WIDTH,
					    actor::WorkerBoat::HEIGHT),
				   true
				   );
  
  stage->create<actor::Cursor>(true);
  stage->create<actor::Cursor>(false);

  stage->create<actor::Island>(Position(0,0,actor::Island::WIDTH, actor::Island::HEIGHT));
  
  for(int j = 0; j < 2; j++)
    for(int i = 0; i < NB_BLOCK_H ; i++)
      stage->create<actor::Warboat>(Position(SIZE_BLOCK_W + j * (NB_BLOCK_W - 3) * SIZE_BLOCK_W +
					     (SIZE_BLOCK_W / 2.f - actor::Boat::WIDTH / 2.f),
  					     i*SIZE_BLOCK_H +
					     (SIZE_BLOCK_H/2.f - actor::Boat::HEIGHT/2.f),
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
