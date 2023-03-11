#include "Entity.h"



Entity::Entity()
{
	is_alive = false;
}
Entity::~Entity()
{
}
void Entity::Init(float posx, float posy, float w, float h, float s)
{
	x = posx;
	y = posy;
	width = w;
	height = h;
	speed = s;
	is_alive = true;
}
void Entity::GetRect(int *posx, int *posy, int *w, int *h)
{
	*posx = x;
	*posy = y;
	*w = width;
	*h = height;
}
float Entity::GetX()
{
	return x;
}
float Entity::GetY()
{
	return y;
}
float Entity::GetDX()
{
	return dx;
}
float Entity::GetDY()
{
	return dy;
}
float Entity::GetWidth()
{
	return width;
}
float Entity::GetHeight()
{
	return height;
}
float Entity::GetSpeed() {
	return this->speed;
}


void Entity::SetX(float posx)
{
	x = posx;
}
void Entity::SetY(float posy)
{
	y = posy;
}
void Entity::SetDX(float dir_x)
{
	dx = dir_x;
}
void Entity::SetDY(float dir_y)
{
	dy = dir_y;
}

void Entity::SetSpeed(float speed) {
	this->speed = speed;
}

void Entity::ShutDown()
{
	is_alive = false;
}
bool Entity::IsAlive()
{
	return is_alive;
}

void Entity::Move()
{
		
	x += this->dx * speed;
	y += this->dy * speed;
	
}
void Entity::SetDir(float dx, float dy) {
	this->dx = dx;
	this->dy = dy;
}


bool Entity::Collide(Entity other) {

	if (this->GetX() >= other.GetX() && this->GetX() + this->GetWidth() <= other.GetX() + other.GetWidth()
		&& this->GetY() <= other.GetY() && this->GetY() + this->GetHeight() >= other.GetY() + other.GetHeight()
		) {
	
		return true;
	
	}
	return false;
}