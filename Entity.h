#pragma once
class Entity
{
public:
	Entity();
	~Entity();

	void Init(float posx, float posy, float w, float h, float s);
	void GetRect(int *posx, int*posy, int*w, int*h);
	void SetX(float posx);
	void SetY(float posy);
	void SetDX(float posx);
	void SetDY(float posy);
	void SetSpeed(float speed);
	float  GetX();
	float  GetY();
	float  GetDX();
	float  GetDY();
	float  GetWidth();
	float  GetHeight();
	float  GetSpeed();
	void ShutDown(); 
	bool IsAlive();
	void Move();
	void SetDir(float dx, float dy);
	bool Collide(Entity other);


private:
	float x, y, dx, dy;
	float width, height;
	float speed;
	bool is_alive;
};

