#pragma once
#include "gameObj.hpp"

bool snakeCollidedWithBoundary(const Snake &s);
bool snakeCollidedWithFood(Point snake,Point food);
bool snakeCollidedWithSpider(const Snake &snake,const Spider &spider);
bool snakeCollidedWithSnake(const Snake &snake);