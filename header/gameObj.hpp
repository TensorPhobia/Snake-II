#pragma once
#include <curses.h>
#include <list>
#include <vector>

struct Point{
    int x,y;
    Point(int x, int y):x(x),y(y){};
    Point():x(0),y(0){};
};

class Spider{
    WINDOW *win;
    public:
    std::vector<std::vector<std::vector<char>>> asciiArt;
    short sprite;
    int Xoffset,Yoffset;
    bool isAlive;
    void pickEmptySpot();
    void randomizeSprite();
    void draw();
    void clear();
    void reset();
    Spider(WINDOW *WIN);
    void setIsAlive(bool flag);
};

class Food{
    WINDOW *win;
    public:
    Point pos;
    Food(WINDOW *WIN);
    void pickEmptySpot();
    void draw();
    void clear();  
    void reset();
};

class Snake{
    std::list<Point> body;
    public:
    Snake(WINDOW *WIN);
    WINDOW *win;
    Point cursor;
    void draw(Point p);
    void erase(Point p);
    void clear();
    void pickEmptySpot();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void push_front(Point p);
    void pop_front();
    void push_back(Point p);
    void reset();
    Point front();
    Point back();
};
