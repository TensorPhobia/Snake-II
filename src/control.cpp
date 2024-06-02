#include "../header/control.hpp"
#include <curses.h>

bool isLegalMove(int key, int lastKey) {
  if (lastKey == KEY_UP && key == KEY_DOWN ||
      lastKey == KEY_DOWN && key == KEY_UP ||
      lastKey == KEY_LEFT && key == KEY_RIGHT ||
      lastKey == KEY_RIGHT && key == KEY_LEFT) {
    return false;
  }
  return true;
}

bool isArrow(int key){
    return key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT;
}