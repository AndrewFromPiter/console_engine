#include "GAME.h"
#include "sprites.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <Windows.h>
#include <chrono>
#include <vector>
#include <fstream>

int findMin(std::vector<int>& vec) {
    if (vec.empty()) return -1;
    int minIndex = 0;
    for (size_t i = 1; i < vec.size(); i++) {
        if (abs(vec[i]) < abs(vec[minIndex])) {
            minIndex = i;
        }
    }
    return minIndex;
}



GAME::GAME() :     //(double x, double y, std::vector<std::string> spr,double r,double rm) параметры инициализации
    screen(),
    floor(0, 2, mainFloorSprite, 0, 0),
    player(0, 0, hero1, 0, 1.0 / 60),
    cube(45, 13, cubeSprite, 0, 1.0 / 200),
    pf1( 16, 3 ),
    pf2( 32,8 ),
    pf3( 48,12 ),
    pf4(64,20)   {   // список инициализаций
    work = true;

    

    objects = { &player,&cube,&floor,&pf1,&pf2,&pf3,&pf4 };
}

void GAME::process() {
    using clock = std::chrono::steady_clock;
    auto frame_duration = std::chrono::milliseconds(static_cast<int>(1000 / FPS));
    
    while (work) {
        auto frame_start = clock::now();
        
        

        
        applyAll(objects);
        for (size_t i = 0;i < objects.size();i++) {
            screen.draw(*objects[i]);
        }
        screen.conclusion(player,cube,*this); //player and cube только для вывода статов



        auto frame_end = clock::now();
        auto elapsed = frame_end - frame_start;

        if (elapsed < frame_duration) {
            std::this_thread::sleep_for(frame_duration - elapsed);
        }
    }
}

void GAME::check_buttom(object &obj) {
    
    left = false;
    right = false;
    up = false;
    circle = false;
    
    if (GetAsyncKeyState('A') & 0x8000) left = true;
    if (GetAsyncKeyState('D') & 0x8000) right = true;
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) up = true;
    if (GetAsyncKeyState('R') & 0x8000) circle = true;

    while (_kbhit()) {
        int currentKey = _getch(); {
            switch (currentKey) {
            case 27: // esc
                work = false;
                break;
            case 8: //backspace
                while (_getch() != 8);
                break;
            }
        }
    }
    
    if(left) obj.setVx(-XspeedForPlayer);
    if(right) obj.setVx(XspeedForPlayer);
    if (up && obj.is_stay()) { 
        obj.setVy(jumpSpeed);
        obj.set_is_stay(false);
    }
    if (circle) obj.circular_motion(-G, XspeedForPlayer*2, 20);
}
void GAME::applyG(std::vector<object*> obj) {
    for (size_t i = 0; i < obj.size();i++) {
        if (obj[i]->Rm != 0) {
            double F = G / obj[i]->Rm;
            obj[i]->getFy(F);
        }
    };
}
bool GAME::check_collision(object& obj1, object& obj2) const {
    colis AABB1 = obj1.get_AABB_coord();
    colis AABB2 = obj2.get_AABB_coord();
    if (AABB1.x1 < AABB2.x2 && AABB1.x2 > AABB2.x1 &&
        AABB1.y1 < AABB2.y2 && AABB1.y2 > AABB2.y1) {
        return true;
    }
    return false;
}
void GAME::processing_collision(object &obj1, object& obj2) {
    collisionDetected = check_collision(obj1, obj2);
    
    if (!collisionDetected) return;
    overlap sidecoll = collisionSide(obj1, obj2);
    
    auto resol = (obj1.resol + obj2.resol)/2;
    double P = 0,V1 = 0,V2 = 0;
    
    switch (sidecoll.sidelap) {
    case side::up:
        P = (1 + resol) * (obj1.Vy - obj2.Vy) / (obj1.Rm + obj2.Rm);
        V1 = obj1.Vy - (P * obj1.Rm);
        V2 = obj2.Vy + (P * obj2.Rm);
        obj1.setVy(V1);
        obj2.setVy(V2);
        if(obj2.Vy <= 0)
            obj2.set_is_stay(true);
        break;
    case side::down:
        P = (1 + resol) * (obj1.Vy - obj2.Vy) / (obj1.Rm + obj2.Rm);
        V1 = obj1.Vy - (P * obj1.Rm);
        V2 = obj2.Vy + (P * obj2.Rm);
        obj1.setVy(V1);
        obj2.setVy(V2);
        if (obj1.Vy <= 0)
            obj1.set_is_stay(true);
        break;
    case side::right:
        P = (1 + resol) * (obj1.Vx - obj2.Vx) / (obj1.Rm + obj2.Rm);
        V1 = obj1.Vx - (P * obj1.Rm);
        V2 = obj2.Vx + (P * obj2.Rm);
        obj1.setVx(V1);
        obj2.setVx(V2);
        break;
    case side::left:
        P = (1 + resol) * (obj1.Vx - obj2.Vx) / (obj1.Rm + obj2.Rm);
        V1 = obj1.Vx - (P * obj1.Rm);
        V2 = obj2.Vx + (P * obj2.Rm);
        obj1.setVx(V1);
        obj2.setVx(V2);
        break;
    }
    //сдвиг
    double correction = 0.1;
    switch (sidecoll.sidelap) {
    case side::up:
        obj1.XYcorrection(0, -sidecoll.depth-correction);
        break;
    case side::down:
        obj1.XYcorrection(0, sidecoll.depth + correction);
        break;
    case side::left:
        obj1.XYcorrection(sidecoll.depth + correction,0 );
        break;
    case side::right:
        obj1.XYcorrection(-sidecoll.depth - correction, 0);
        break;
    }

}
overlap GAME::collisionSide(object& obj1, object& obj2) {
    colis aabb1 = obj1.get_AABB_coord();
    colis aabb2 = obj2.get_AABB_coord();

    std::vector<int> overlapted(4);        //относительно второго
    overlapted[0] = (aabb2.x1 - aabb1.x2); //справа
    overlapted[1] = (aabb2.y1 - aabb1.y2); //сверху
    overlapted[2] = (aabb2.x2 - aabb1.x1); //слева 
    overlapted[3] = (aabb2.y2 - aabb1.y1); //снизу
    
    int index = findMin(overlapted);
    overlap res{side::none,0.0};
    switch (index) {                        //сдвиг
    case 0:
        res.sidelap = side::right;
        res.depth = overlapted[0];
        break;
    case 1:
        res.sidelap = side::up;
        res.depth = overlapted[1];
        break;
    case 2:
        res.sidelap = side::left;
        res.depth = overlapted[2];
        break;
    case 3:
        res.sidelap = side::down;
        res.depth = overlapted[3];
        break;
    }
    return res;
    
}



void GAME::applytime(std::vector<object*> obj) {
    for (size_t i = 0; i < obj.size();i++) {
        obj[i]->time();
        
    }
}
void GAME::updateV(std::vector<object*> obj) {
    for (size_t i = 0; i < obj.size();i++)
        obj[i]->PREupdate();
}
void GAME::updatePos(std::vector<object*> obj) {
    for (size_t i = 0; i < obj.size();i++)
        obj[i]->update();
}

void GAME::applyAll(std::vector<object*> obj) {
    applytime(obj);
    applyG(obj);
    check_buttom(player);
    updateV(obj);

    for (auto& obj_ptr : obj) {            //сброс флага stay
        obj_ptr->set_is_stay(false);
    }

    for (int n = 0;n < 3;n++) {
        for (size_t i = 0;i < obj.size();i++) {
            for (size_t j = i + 1;j < obj.size();j++)
                processing_collision(*obj[i], *obj[j]);
        };
    }
    updatePos(obj);

}

//void GAME::Load_Level(const std::string& filename) {

