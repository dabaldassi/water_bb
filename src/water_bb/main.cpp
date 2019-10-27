#include <gameEngine/game.h>
#include <gameEngine/actor/static.h>
#include <gameEngine/ihm/color.h>
#include <gameEngine/actor/items/item.h>
#include <gameEngine/actor/items/weapon.h>
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

  const float damage = 20.f;

  if(rabbit->isOnTheGround()) {
    if(boat) {
      rabbit->pick(boat);
      rabbit->playSound(0);
    }
    else {
      b->kill();
      rabbit->takeDamage(damage);

      if(rabbit->isDead()) {
	bool * data;
	getDataElement(b->elem(), (void **)&data);

	if(*data) actor::Warboat::nb_boat_1 = 0;
	else      actor::Warboat::nb_boat_2 = 0;

	delete data;
      }
    }
  }
}

void effectRabbit(actor::Actor * actor)
{
  actor::Item * rabbit = static_cast<actor::Item *>(actor);

  rabbit->stage()->endStage();
  *static_cast<bool *>(rabbit->stage()->getData()) = rabbit->getPosition<b2Vec2>().x < actor::Warboat::WIDTH;
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

void canonEffect(actor::Actor * actor, actor::Actor * b)
{
  b2Vec2 pos = b->body()->GetPosition();
  const b2Vec2 & goal = static_cast<actor::Warboat *>(b)->getGoalCanon();
  
  actor::Moveable * ball = &actor->stage()->create<actor::Moveable>("ball", 1, Position(pos.x, pos.y, 8,8));

  float  deltaX = abs(goal.x - pos.x);
  float  deltaY = goal.y - pos.y;
  float  angle = atanf(deltaY / deltaX);
  
  ball->body()->SetLinearVelocity(b2Vec2(((goal.x < pos.x)?-1:1) * 10*cosf(angle), 10*sinf(angle)));
  pos.x += ((goal.x < pos.x)?-1:1) * actor::Warboat::WIDTH / Viewport::METER_TO_PIXEL;
  ball->body()->SetTransform(pos, 0);
  ball->body()->GetFixtureList()->SetDensity(0.7f);

  b2MassData mass;
  ball->body()->GetMassData(&mass);
  mass.mass = 100;
  ball->body()->SetMassData(&mass);
  
  ball->loadSprite(Color::black);
  bool * team = new bool;
  *team = static_cast<actor::Warboat *>(b)->team();
  setDataElement(ball->elem(), (void *)team);
  setFreeDataElement(ball->elem(), NULL);
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

  createBlock(0, 0, w, h, sea_color, 0, 5);

  auto pos = [&SIZE_BLOCK_W, &SIZE_BLOCK_H](float x,float y, float w, float h) {
    return Position(SIZE_BLOCK_W * x + (SIZE_BLOCK_W - w) / 2.f,
		    SIZE_BLOCK_H * y +  (SIZE_BLOCK_H - h) / 2.f,
		    w, h);
  };

  auto createWorkerBoat = [&stage,&SIZE_BLOCK_W,&SIZE_BLOCK_H,&pos](int w,int h) {
    stage->create<actor::WorkerBoat>(pos(w, h,actor::WorkerBoat::WIDTH,actor::WorkerBoat::HEIGHT),
				     w == 3 );
  };

  actor::Item * rabbit =
    &stage->create<actor::Item>("rabbit", pos(NB_BLOCK_W>>1, NB_BLOCK_H>>1,85,85),10000.f, 100.f);
  
  rabbit->loadSprite(RABBIT);
  rabbit->addCollisionOnStatement(collisionRabbit);
  rabbit->addEffectStatement(effectRabbit);
  rabbit->addSound(RABBIT_ALERT);

  b2Filter filter;
  filter.maskBits = 0xffff;
  filter.categoryBits = 0x1000;
  rabbit->body()->GetFixtureList()->SetFilterData(filter);
  
  createWorkerBoat(3,5);
  createWorkerBoat(3,2);
  createWorkerBoat(17,5);
  createWorkerBoat(17,2);
  
  stage->create<actor::Cursor>(true);
  stage->create<actor::Cursor>(false);

  stage->create<actor::Island>(pos(5,0,actor::Island::WIDTH, actor::Island::HEIGHT));
  stage->create<actor::Island>(pos(15,0,actor::Island::WIDTH, actor::Island::HEIGHT));
  stage->create<actor::Island>(pos(10,7 ,actor::Island::WIDTH, actor::Island::HEIGHT));

  for(int j = 0; j < 2; j++) {
    for(int i = 0; i < NB_BLOCK_H ; i++) {
      actor::Warboat *warboat =
	&stage->create<actor::Warboat>(pos(1 + j * (NB_BLOCK_W - 3),i, actor::Boat::WIDTH, actor::Boat::HEIGHT),
				       j==0);
      actor::Weapon * canon = &stage->create<actor::Weapon>("canon", Position(0,0,100,100), warboat, 500);
      canon->set(20.f, 750.f);
      canon->addEffectStatement(canonEffect);
    }
  }
  
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
