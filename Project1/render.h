#ifndef RENDER_H
#define RENDER_H
#include <vector>
#include <cstddef>

class object;
class GAME;

class render {
   
    size_t volume_frame = 0;

    
    std::vector<std::vector<char>> area;

public:
    static const size_t height = 40;
    static const size_t width = 178;

    render();
    void conclusion(object& player,object &cube, const GAME& game);
    void draw(object& obj);
    void clearArea(std::vector<std::vector<char>>& area);
};

#endif