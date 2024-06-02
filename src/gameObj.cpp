#include "../header/gameObj.hpp"
#include "../header/settings.hpp"
#include <cstdlib>
#include <curses.h>


#pragma  region Spider_Logic

Spider::Spider(WINDOW *WIN){
    win = WIN;
    Xoffset = Yoffset = 0;
    isAlive = false;
    sprite = 0;
    asciiArt = {
        {
            {'_','\\','(',' ',')','/','_'},
            {' ','/','(','O',')','\\',' '},
        },
        {
                {'_','\\','(','o',')','/','_'},
                {' ','/','(','_',')','\\',' '},
        },


    };

}


void Spider::reset(){
    clear();
    Xoffset = Yoffset = 0;
    isAlive = false;
    sprite = 0;
}

void Spider::setIsAlive(bool flag){
    isAlive = flag;
}


void Spider::randomizeSprite(){
    sprite = rand() % asciiArt.size();
}

void Spider::pickEmptySpot(){
    bool isOccpied;
    do {
        Xoffset = rand()%getmaxx(win);
        Yoffset = rand()%getmaxy(win);
        isOccpied = false;

        for (int lins = 0; lins < asciiArt[sprite].size() && !isOccpied; lins++) {
            for (int cols = 0; cols < asciiArt[sprite][0].size() && !isOccpied; cols++) {
                isOccpied = (mvwinch(win,lins+Yoffset,cols+Xoffset) & A_CHARTEXT) != ' ';
            }
        }

    }while (isOccpied);
}


void Spider::draw(){
    wattron(win, COLOR_PAIR(2));
    for (int lins = 0; lins < asciiArt[sprite].size(); lins++) {
        for (int cols = 0; cols < asciiArt[sprite][0].size(); cols++) {
            mvwaddch(win,lins+Yoffset,cols+Xoffset,asciiArt[sprite][lins][cols]);
        }
    }
    wattroff(win, COLOR_PAIR(2));
    wrefresh(win);
}

void Spider::clear(){    
    for (int lins = 0; lins < asciiArt[sprite].size(); lins++) {
        for (int cols = 0; cols < asciiArt[sprite][0].size(); cols++) {
            mvwaddch(win,lins+Yoffset,cols+Xoffset,' ');
        }
    }
    
    wrefresh(win);
}

#pragma  endregion Spider_Logic


#pragma  region Food_Logic

Food::Food(WINDOW *WIN){
    win = WIN;
    pickEmptySpot();
    draw();
}

void Food::reset(){
    clear();
    pickEmptySpot();
    draw();
}


void Food::draw(){
    wattron(win, COLOR_PAIR(3));
    mvwaddch(win,pos.y, pos.x, FOOD_CHAR);
    wattroff(win, COLOR_PAIR(3));
    wrefresh(win);
}

void Food::clear(){
    mvwaddch(win,pos.y, pos.x, ' ');
    wrefresh(win);
}

void Food::pickEmptySpot(){
    chtype ch;
    do{
        pos.x = rand()%getmaxx(win);
        pos.y = rand()%getmaxy(win);
    }while( (ch = (mvwinch(win,pos.y,pos.x) & A_CHARTEXT)) != ' ');
}

#pragma  endregion Food_Logic


#pragma  region Snake_Logic

Snake::Snake(WINDOW *WIN){
    win = WIN;
    cursor = Point(1,getmaxy(win)/2);
    push_front(cursor);
}

void Snake::reset(){
    clear();
    cursor = Point(1,getmaxy(win)/2);
    push_front(cursor);
}


void Snake::draw(Point p){
    wattron(win, COLOR_PAIR(4));
    mvwaddch(win,p.y,p.x, 'x');
    wattroff(win, COLOR_PAIR(4));
    wrefresh(win);
}

void Snake::erase(Point p){
    mvwaddch(win,p.y,p.x, ' ');
    wrefresh(win);
}

void Snake::moveUp(){
    cursor.y--;
    if(cursor.y < 0){
        cursor.y+=getmaxy(win);
    }
}

void Snake::moveDown(){
    cursor.y++;
    if(cursor.y >= getmaxy(win)){
        cursor.y-=getmaxy(win);
    }

}

void Snake::moveLeft(){
    cursor.x--;
    if(cursor.x < 0){
        cursor.x+=getmaxx(win);
    }
}

void Snake::moveRight(){
    cursor.x++;
    if(cursor.x >= getmaxx(win)){
        cursor.x-=getmaxx(win);
    }
}

void Snake::push_front(Point p){
    draw(p);
    body.push_front(p);
}

void Snake::push_back(Point p){
    draw(p);
    body.push_back(p);
}

void Snake::pop_front(){
    erase(body.front());
    body.pop_front();
}

void Snake::clear(){
    for (auto point : body) {
        erase(point);
    }
    body.clear();
}


Point Snake::front(){
    return body.front();
}

Point Snake::back(){
    return body.back();
}

#pragma  endregion Food_Logic
