#include "render.h"
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include "GAME.h"
#include "object.h"

// массив area
void render::clearArea(std::vector<std::vector<char>> &area) {
    area = std::vector<std::vector<char>>(height, std::vector<char>(width, ' '));
}

void setConsoleSize(int width, int height) {
    std::string command = "mode con cols=" + std::to_string(width) +
        " lines=" + std::to_string(height + 5);
    system(command.c_str());
}

render::render() {
    clearArea(area);
    setConsoleSize(width,height);

}



void render::conclusion(object &player,object &cube, const GAME &game) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hStdOut, coord);

    
    std::string buffer;
    buffer.reserve(height * (width + 10)); 

    for (int i = height - 1; i >= 0; i--) {
        buffer.append(area[i].data(), width);
        buffer += '\n';
    }

    //пол
    /*
    for (int j = 0;j < width;j++)
        buffer += 'T';
    buffer += "\n";*/

    // Статусная строка
    buffer += "Status player: x: " + std::to_string(player.position.x) +
              " y: " + std::to_string(player.position.y * 2) +
              " frame: " + std::to_string(volume_frame) +
              " dt: " + std::to_string(player.dt);
    
    buffer += " Keys:[";
    buffer += game.left ? "A_" : "__";
    buffer += game.right ? "D_" : "__";
    buffer += game.up ? "SPACE_" : "______";
    buffer += game.circle ? "R" : "_";
    buffer += "]" ;
    buffer += '\n';

    buffer += "Status cube: x: " + std::to_string(cube.position.x) +
        " y: " + std::to_string(cube.position.y * 2)+
        " dt: " + std::to_string(cube.dt);
    buffer += player.is_stay() ? "\nplayer is stay  " : "\nplayer not stay   ";
    //печать
    std::cout << buffer;
    
    volume_frame++;
    clearArea(area);
    buffer.clear();
}


void render::draw(object &obj) {
    const std::vector<std::vector<char>>* sprite = obj.sprite();

    if (obj.position.x < 0) obj.position.x = 0;
    //if (obj.position.y < 0) obj.position.y = 0;
    /*
    if (obj.position.x > width - (*sprite)[0].size() - 2) obj.position.x = width - (*sprite)[0].size() - 2;
    if (obj.position.y > height - sprite->size()) obj.position.y = height - sprite->size();
    */
    point pos = obj.getPosition();

    

    for (size_t i = 0;i < sprite->size();i++) {
        for (int j = 0;j < (*sprite)[i].size();j++) {
            int y_pos = static_cast<int>(i) + pos.Ry();
            int x_pos = static_cast<int>(j) + pos.Rx();
            
            if (y_pos >= 0 && y_pos < height &&
                x_pos >= 0 && x_pos < width) {

                area[y_pos][x_pos] = (*sprite)[i][j];

            }
        }
    }

}

