#include <cmath>
#include <Box2D/Box2D.h>

#include "boat.h"
#include "island.h"
#include "cursor.h"

using actor::Boat;

Boat::Boat(const std::string name, float life, const Position & p, bool team):Moveable(name, life,p)
{
  b2MassData mass;
  b2Filter filter;

  _body->GetMassData(&mass);
  _body->SetLinearDamping(50.f);
  _body->SetAngularDamping(50.f);
 
  mass.mass = 50000;
  
  filter.categoryBits = CATEGORY;
  filter.maskBits = Cursor::CATEGORY | Island::CATEGORY;// | 0xffff;

  _body->SetMassData(&mass);
  _body->GetFixtureList()->SetFilterData(filter);
  
  _isMoving = false;
  _selected = false;
  _speed = 3.f;
  _food = 100.f;
  _team = team;
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

    if(abs(_body->GetAngle()) < EPSILON) _isMoving = false;
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

    if(_selected) display();
    else this->clearDisplay();
  }
}

void Boat::display() const
{
  std::cout << "Life : " << _life << "\n" << "Food : " << _food << "\n";
}
