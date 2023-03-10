#include "Entity.h"



Entity::Entity()
{
	is_alive = false;
}
Entity::~Entity()
{
}
void Entity::Init(int posx, int posy, int w, int h, int s) 
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
int Entity::GetX()
{
	return x;
}
int Entity::GetY()
{
	return y;
}
int Entity::GetDX()
{
	return dx;
}
int Entity::GetDY()
{
	return dy;
}
int Entity::GetWidth()
{
	return width;
}
int Entity::GetHeight()
{
	return height;
}
int Entity::GetSpeed() {
	return this->speed;
}


void Entity::SetX(int posx)
{
	x = posx;
}
void Entity::SetY(int posy)
{
	y = posy;
}
void Entity::SetDX(int dir_x)
{
	dx = dir_x;
}
void Entity::SetDY(int dir_y)
{
	dy = dir_y;
}

void Entity::SetSpeed(int speed) {
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
void Entity::SetDir(int dx, int dy) {
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