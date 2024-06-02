#include "../header/collision.hpp"
#include "../header/settings.hpp"
#include <curses.h>

bool snakeCollidedWithFood(Point snake,Point food){
    return snake.x == food.x && snake.y == food.y;
}

bool snakeCollidedWithBoundary(const Snake &s){
    if(s.cursor.x == 0 || s.cursor.x == getmaxx(s.win)-1 || s.cursor.y == 0 || s.cursor.y == getmaxy(s.win)-1)
        return true;
    return false;
}

bool snakeCollidedWithSpider(const Snake &snake,const Spider &spider){
    if(snake.cursor.x >= spider.Xoffset && snake.cursor.x <= spider.Xoffset + spider.asciiArt[spider.sprite][0].size()
        && snake.cursor.y >= spider.Yoffset && snake.cursor.y <= spider.Yoffset + spider.asciiArt[spider.sprite].size()
    ){
        return true;
    } 
    return false;
}

bool snakeCollidedWithSnake(const Snake &snake){
    if((mvwinch(snake.win,snake.cursor.y,snake.cursor.x) & A_CHARTEXT) == SNAKE_CHAR
    ){
        return true;
    } 
    return false;
}