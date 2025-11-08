#include "object.h"
#include <cmath>
#include <vector>
#include <chrono>
#include <iostream>
#include <Windows.h>



void resprite(std::vector<std::vector<char>>& sprite) {
	std::vector<std::vector<char>> copy = sprite;
	for (size_t i = 0;i < sprite.size();i++)
		sprite[i] = copy[sprite.size() - 1 - i];
}
void toSprite(const std::vector<std::string>& tex, std::vector<std::vector<char>>& real) {

	real.resize(tex.size());

	for (size_t i = 0;i < real.size();i++) {
		real[i].resize(tex[i].size());

		for (size_t j = 0;j < real[i].size();j++) {
			real[i][j] = tex[i][j];
		}
	}
}

object::object(double x, double y, std::vector<std::string> spr,double r,double rm): position(x, y) {
	resol = r;
	Rm = rm;

	TEXsprite = spr;

	realSprite;

	Fx = 0;
	Fy = 0;

	
	toSprite(TEXsprite,realSprite);
	resprite(realSprite);

	

	lastTime = std::chrono::steady_clock::now();
}
object::object(double x, double y) : position(x, y) {
	resol = 0;
	Rm = 0;

	TEXsprite = platformSprite;

	realSprite;

	Fx = 0;
	Fy = 0;


	toSprite(TEXsprite, realSprite);
	resprite(realSprite);



	lastTime = std::chrono::steady_clock::now();
}

const std::vector<std::vector<char>>* object::sprite() const{
	return &realSprite;
}

void object::time() {
	auto now = std::chrono::steady_clock::now();
	
	auto dt_duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime);
	dt = dt_duration.count() / 1000.0;

	lastTime = now;  
}




void object::circular_motion(double G,double s, double r) {
	if (position.f > 3.142 * 2) position.f = 0;
	position.f += s/r * dt;
	position.x = 40 + r * cos(position.f);
	position.y = (40 + r * sin(position.f))/4;

	setVy(G*dt);
}
colis object::get_AABB_coord() {
	colis AABB;
	double x1 = position.x;
	double y1 = position.y;
	double x2 = position.x + (double)realSprite[0].size();
	double y2 = position.y + (double)realSprite.size();
	AABB.set(x1,y1,x2,y2);
	return AABB;
}


void object::getFx(double F) {
	Fx += F;
}
void object::getFy(double F) {
	Fy += F;
}
void object::PREupdate() {
	Ax = Fx * Rm;
	Vx += Ax * dt;
	
	//Vx -= 1.0 * dt * (Vx > 0 ? 1 : -1); // Постоянное замедление
	Vx -= 0.3 * Vx;

	Ay = Fy * Rm;
	Vy += Ay * dt;
	
	Fx = 0;
	Fy = 0;
	Ax = 0;
	Ay = 0;
}
void object::update() {


	position.x += Vx * dt;
	position.y += Vy * dt;
}
void object::setVy(double V) {
	Vy = V;
}
void object::setVx(double V) {
	Vx = V;
}
void object::XYcorrection(double dx, double dy) {
	position.x += dx;
	position.y += dy;
}