#include <cmath>
#include <Box2D/Box2D.h>
#include <random>

#include "boat.h"
#include "island.h"
#include "cursor.h"
#include "sounds.h"

using actor::Boat;

Boat::Boat(const std::string name, float life, const Position & p, bool team):Moveable(name, life,p)
{
  b2MassData mass;
  b2Filter filter;
  std::vector<std::string> sound = {SELECTION1, SELECTION2, SELECTION3, SELECTION4, SELECTION5,
				    DEPART1,DEPART2, DEPART3, DEPART4, DEPART5,DEPART6, DEADBOAT};


  std::for_each(sound.begin(), sound.end(), [&](std::string s) {this->addSound(s);});
  
  _body->GetMassData(&mass);
  _body->SetLinearDamping(50.f);
  _body->SetAngularDamping(50.f);
 
  mass.mass = 50000;
  
  filter.categoryBits = CATEGORY;
  filter.maskBits = Cursor::CATEGORY | Island::CATEGORY | Boat::CATEGORY | 0x1000;// | 0xffff;

  _body->SetMassData(&mass);
  _body->GetFixtureList()->SetFilterData(filter);
  
  _isMoving = false;
  _selected = false;
  _speed = 3.f;
  _food = 100.f;
  _team = team;
  _timeFood = 10000;
  _timerFood = 0.f;
  _lifeBar = NULL;
  _foodBar = NULL;
  
  
}

void Boat::move(float dt)
{
  b2Vec2 velocity = _body->GetLinearVelocity();
  b2Vec2 pos = _body->GetPosition();
  float  deltaX = _goal.x - pos.x;
  float  deltaY = _goal.y - pos.y;
  float  angle = atanf(deltaY / deltaX);

  constexpr float EPSILON = 0.05;

  if(abs(deltaX) < EPSILON && abs(deltaY) < EPSILON) {

    _body->SetAngularVelocity(((_body->GetAngle() > 0)?-1:1)* 0.5);

    if(abs(_body->GetAngle()) < EPSILON) {
      _isMoving = false;
      effect();
    }
  }
  else {
    velocity.x += ((deltaX < 0)?-1:1) * _speed * (1 - abs(angle) / (M_PI / 2));
    velocity.y += ((deltaY < 0)?-1:1) * _speed * (abs(angle) / (M_PI / 2));
  
    _body->SetLinearVelocity(velocity);
    _body->SetTransform(pos, angle);
  }
}

b2Vec2 operator/(const b2Vec2 & vec, float div)
{
  return b2Vec2(vec.x/div, vec.y/div);
}

bool operator!=(const b2Vec2 & vec1, const b2Vec2 & vec2) 
{
  return (vec1.x - vec2.x) > 0.001 || (vec1.y - vec2.y) > 0.001;
}

void Boat::select()
{
  if(!_isMoving || _selected) {
    _selected = !_selected;

    if(_selected) {
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> d(0,100);
      std::string sound[] = {SELECTION1,SELECTION2,SELECTION3,SELECTION4,SELECTION5};

      int id = d(gen);
     
      if(id >= 90 && id <= 97) id = 1;
      else if(id > 97) id = 4;
      else if(id >= 0 && id < 30) id = 0;
      else if(id >= 30 && id < 60) id = 2;
      else id = 3;

      playSound(sound[id]);
      
      display();
    }
    else clearDisplay();
  }
}

void Boat::display()
{
  const b2Vec2 & p = getPosition<b2Vec2>();
  int green[4] = {10,150,10,180};
  int blue[4] = {10,10,150,255};
  
  _lifeBar = new ihm::ProgressBar(Position(p.x, p.y + WIDTH/3, WIDTH - 10, 4),green,_life/100.f);
  _foodBar = new ihm::ProgressBar(Position(p.x, p.y - WIDTH/3, WIDTH - 10, 4),blue,_life/100.f);
}

void Boat::clearDisplay()
{
  delete _lifeBar;
  delete _foodBar;

  _lifeBar = NULL;
  _foodBar = NULL;
}

void Boat::act(float dt)
{
  
  if(_timerFood >= _timeFood) {
    _timerFood = 0;
    _food -= 5;
  }

  if(_lifeBar) *_lifeBar = _life/100.f;
  if(_foodBar) *_foodBar = _food/100.f;

  _timerFood += dt;

  if(_life <= 0 || _food <= 0) {
    kill();
    playSound(DEADBOAT);
  }
  
}

void Boat::refuel(float *food)
{
  int f = 100 - _food;
  
  _food += (*food < f)?*food:f;

  *food = (*food < f)?0.f:*food - f; 
  
}

Boat::~Boat()
{
  if(_lifeBar) delete _lifeBar;
  if(_foodBar) delete _foodBar;
}

void Boat::setGoal(const b2Vec2 & vec)
{
  _goal = vec;
  _isMoving = true;
}

void Boat::go_sound()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> d(0,100);
  std::string sound[] = {DEPART1,DEPART2,DEPART3,DEPART4,DEPART5,DEPART6};

  int id = d(gen);

  if(id >= 50 && (_life <= 15 || _food <= 10)) id = 3;
  else if(id >= 50 && (_food <= 40)) id = 4;
  else if(id < 50 && (_food <= 60)) id = 2;
  else if(id < 40) id = 0;
  else if(id >= 40 && id < 80) id = 1;
  else id = 5;
    
  playSound(sound[id]);
}
