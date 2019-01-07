#include <gameEngine/game.h>
#include <gameEngine/actor/static.h>
#include <gameEngine/ihm/color.h>
#include <gameEngine/actor/items/item.h>
#include <gameEngine/ihm/button.h>
#include <gameEngine/ihm/text.h>
#include <gameEngine/ihm/font.h>

#include "cursor.h"
#include "worker_boat.h"
#include "island.h"
#include "warboat.h"
#include "sprite.h"
#include "sounds.h"

#include <Box2D/Box2D.h>

void collisionRabbit(actor::Actor * actor, actor::Actor * b)
{
  actor::Item    * rabbit = static_cast<actor::Item *>(actor);
  actor::Warboat * boat = dynamic_cast<actor::Warboat *>(b);

  if(rabbit->isOnTheGround() && boat) {
    boat->pickFlag(rabbit->pick());
    rabbit->playSound(0);
  }
}

void effectRabbit(actor::Actor * actor)
{
  actor::Item * rabbit = static_cast<actor::Item *>(actor);

  rabbit->stage()->endStage();

  *static_cast<bool *>(rabbit->stage()->getData()) = rabbit->body()->GetPosition().x * Viewport::METER_TO_PIXEL < actor::Warboat::WIDTH;
}

bool end_menu(float dt)
{
  DataWindow * d;

  getDataWindow((void **)&d);
  
  return d->param == 1;
}

void replay(Element * e, int button)
{
  DataWindow * d;

  getDataWindow((void **)&d);

  d->param = 2;
}

void quit(Element * e, int button)
{
  DataWindow * d;

  getDataWindow((void **)&d);

  d->param = 3;
}

void motion(Element * e)
{
  setTextColorElement(e, Color::red);
}

void exitmotion(Element * e)
{
  setTextColorElement(e, Color::white);
}

void ending(Stage * stage, void * data) {
  DataWindow * dw;
  int w, h;
  std::string msg("Victoire du joueur ");
  int trans[] = {0,0,0,0};
  Element * e;

  getDimensionWindow(&w, &h);
  
  msg = msg+ std::to_string(!*static_cast<bool *>(data)+1);
  
  getDataWindow((void **)&dw);

  while(stage->actors().size() > 0) {
    stage->actors().pop_back();
  }
 
  createText(w/4, h/4, 1200, 100, 50, FREE_MONO_BOLD, msg.c_str(), Color::white, SANDAL2_BLENDED, 0, 0);
  e = createButton(w/4, h/2, 200, 100, 50, FREE_MONO_BOLD, "Rejouer", Color::white, SANDAL2_BLENDED, trans, 0, 0);
  addClickableElement(e, rectangleClickable(0, 0, 1, 1), 0);
  setOnClickElement(e, replay);
  setOnMouseMotionElement(e, motion);
  setUnMouseMotionElement(e, exitmotion);
  
  e = createButton(3*w/4, h/2, 200, 100, 50, FREE_MONO_BOLD, "Quitter", Color::white, SANDAL2_BLENDED, trans, 0, 0);
  addClickableElement(e, rectangleClickable(0, 0, 1, 1), 0);
  setOnClickElement(e, quit);
  setOnMouseMotionElement(e, motion);
  setUnMouseMotionElement(e, exitmotion);

  dw->game->event_manager(end_menu);
  
  if(dw->param == 2) {
    dw->param = 1;
    clearDisplayCode(0);
    stage->disableEnd();
    stage->generate();
  }
  else 
    dw->param = 0;
  
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

  b2Body * b = stage->world().GetBodyList();

  while(b) std::cout << "body" << "\n";

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
  rabbit->addSound(RABBIT_ALERT);
  
  stage->create<actor::WorkerBoat>(Position(SIZE_BLOCK_W / 2.f - actor::WorkerBoat::WIDTH / 2.f,
					    4*SIZE_BLOCK_H +
					    (SIZE_BLOCK_H/2.f - actor::WorkerBoat::HEIGHT / 2.f),
					    actor::WorkerBoat::WIDTH,
					    actor::WorkerBoat::HEIGHT),
				   true
				   );
  
  stage->create<actor::Cursor>(true);
  stage->create<actor::Cursor>(false);

  stage->create<actor::Island>(Position(4*SIZE_BLOCK_W,0,actor::Island::WIDTH, actor::Island::HEIGHT));
  stage->create<actor::Island>(Position(14*SIZE_BLOCK_W,0,actor::Island::WIDTH, actor::Island::HEIGHT));

  stage->create<actor::Island>(Position(9*SIZE_BLOCK_W,7*SIZE_BLOCK_H ,actor::Island::WIDTH, actor::Island::HEIGHT));

  actor::Warboat::nb_boat_1 = 0;
  actor::Warboat::nb_boat_2 = 0;
  
  for(int j = 0; j < 2; j++)
    for(int i = 0; i < NB_BLOCK_H ; i++)
      stage->create<actor::Warboat>(Position(SIZE_BLOCK_W + j * (NB_BLOCK_W - 3) * SIZE_BLOCK_W +
					     (SIZE_BLOCK_W / 2.f - actor::Boat::WIDTH / 2.f),
  					     i*SIZE_BLOCK_H +
					     (SIZE_BLOCK_H/2.f - actor::Boat::HEIGHT/2.f),
  					     actor::Boat::WIDTH,
  					     actor::Boat::HEIGHT),
  				    j==0);
  
  stage->addMusic(MUSIC_MAIN_PHASE);
  stage->playMusic(0);
  bool * dataEnd = new bool;

  stage->setData(dataEnd);
  
  stage->setEndStage([](void * data) {
      bool * d = static_cast<bool *>(data);
      if(actor::Warboat::nb_boat_1 == 0) *d = false;
      else if(actor::Warboat::nb_boat_2 == 0) *d = true;
      else return false;
      return true;});

  stage->endCallback(ending);

  
}

int main()
{
  Game game(1920,1080,"Water Boat Battle");
  
  game.run(generate);
  
  return 0;
}
