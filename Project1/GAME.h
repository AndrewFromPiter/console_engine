#ifndef GAME_H
#define GAME_H
#include "object.h"
#include "render.h"
#include <vector>

enum class side {
	right,
	left,
	up,
	down,
	none
};
class overlap {
public:
	side sidelap;
	double depth;
	overlap(side s, double d) { sidelap = s;depth = d; };
};
class GAME {
	bool work = false;
	std::vector<object*> objects;
	object player, cube, floor, pf1, pf2, pf3,pf4;
	render screen;
	
	double XspeedForPlayer = 25;
	double jumpSpeed = 45;
	double G = -150;
	double Friction = 1;
	

	//void Load_Level(const std::string& filename);
public:
	GAME();

	void process();

	void check_buttom(object& obj);

	void applytime(std::vector<object*> obj);

	void applyG(std::vector<object*> obj);

	void updateV(std::vector<object*> obj);
	void updatePos(std::vector<object*> obj);

	void applyAll(std::vector<object*> objects);

	bool check_collision(object& obj1, object& obj2) const;
	void processing_collision(object& obj1, object& obj2);
	overlap collisionSide(object& obj1, object& obj2);

	bool collisionDetected = false;

	bool left = false;
	bool right = false;
	bool up = false;
	bool circle = false;

	float FPS = 60;


};



#endif