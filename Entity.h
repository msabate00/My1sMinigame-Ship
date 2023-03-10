#pragma once
class Entity
{
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int s);
	void GetRect(int *posx, int *posy, int *w, int *h);
	void SetX(int posx);
	void SetY(int posy);
	void SetSpeed(int speed);
	int  GetX();
	int  GetY();
	int  GetWidth();
	int  GetHeight();
	int  GetSpeed();
	void ShutDown(); 
	bool IsAlive();
	void Move(int dx, int dy);
	bool Collide(Entity other);


private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
};

