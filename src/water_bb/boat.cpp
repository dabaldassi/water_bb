#include <cmath>
#include <Box2D/Box2D.h>

#include "boat.h"
#include "cursor.h"

using actor::Boat;

Boat::Boat(const std::string name, float life, const Position & p):Moveable(name, life,p)
{
  b2MassData mass;
  b2Filter filter;

  _body->GetMassData(&mass);
  _body->SetLinearDamping(50.f);
  _body->SetAngularDamping(50.f);
 
  mass.mass = 50000;
  
  filter.categoryBits = CATEGORY;
  filter.maskBits = Cursor::CATEGORY;

  _body->SetMassData(&mass);
  _body->GetFixtureList()->SetFilterData(filter);
  
  _isMoving = true;
  _speed = 3.f;
  
}

void Boat::move(float dt)
{
  b2Vec2 velocity = _body->GetLinearVelocity();
  b2Vec2 pos = _body->GetPosition();
  float  deltaX = _goal.x - pos.x;
  float  deltaY = _goal.y - pos.y;
  float  angle = atanf(deltaY / deltaX);

  constexpr float EPSILON = 0.05;

  if(deltaX < EPSILON && deltaY < EPSILON) {
    _body->SetAngularVelocity(0.5);

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
