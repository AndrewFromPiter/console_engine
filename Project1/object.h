#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include "sprites.h"

class point {

public:

	double f = 0.0;

	double x;
	double y;
	point() : x(0), y(0) {};
	point(double a, double b) : x(a), y(b) {};

	void Xupdate(double dx) { x += dx; }
	void Yupdate(double dy) { y += dy; }

	int Rx() const { 	return static_cast<int>(round(x));	}; 
	int Ry() const { 	return static_cast<int>(round(y)); 	};  
	
	void nullY() { y = 0; };
};

struct colis {
	double x1;
	double y1;

	double x2;
	double y2;

	void set(double a, double b,double c,double d){
		x1=a;
		y1=b;
		x2=c;
		y2=d;
	}
};





class object {
	std::vector<std::string> TEXsprite;
	std::vector<std::vector<char>> realSprite;

	double friction = 0.5;
	
	long double T = (std::chrono::duration_cast<std::chrono::
		milliseconds>(std::chrono::system_clock::
			now().time_since_epoch()).count())/1000;

	

	std::chrono::steady_clock::time_point lastTime;

	bool stay = false;

public:
	double dt = 0;
	double Vy = 0;
	double Ay = 0;
	double Vx = 0;
	double Ax = 0;
	double resol;
	double Rm; // 1/m
	double Fy = 0;
	double Fx = 0;

	

	point position;

	object(double x, double y, std::vector<std::string> spr, double r, double Rm);
	object(double x, double y);

	point getPosition() const { return position; };

	const std::vector<std::vector<char>>* sprite() const;
	void time();

	void getFx(double F);
	void getFy(double F);
	void setVy(double V);
	void setVx(double V);
	void PREupdate();
	void update();
	void XYcorrection(double dx,double dy);

	void circular_motion(double G, double w, double r);
	
	colis get_AABB_coord();

	void set_is_stay(bool s) { stay = s; };
	bool is_stay() const { return stay; }
};


#endif