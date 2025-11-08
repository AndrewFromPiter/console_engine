#ifndef SPRITES_H
#define SPRITES_H

#include <vector>
#include <string>
#include "render.h"
using std::vector;
using std::string;


inline vector<string> hero1 = {
	"  ()  ",
	" /TT\\ ",
	"/ || \\",
	" //\\\\ ",
	"//  \\\\"
};

inline vector<string> cubeSprite = {
	"--------",
	"| * *  |",
	"|  __  |",
	"|______|"
};
inline vector<string> mainFloorSprite = {
	"|" + string(render::width - 2,'^') + "|"
};
inline vector<string> platformSprite = {
	"|ZZZZZZZZZZZZZ|"
};

#endif