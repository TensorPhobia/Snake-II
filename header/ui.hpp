#pragma once
#include <curses.h>
class TitleBar {
  WINDOW *win;
public:
  TitleBar(WINDOW *WIN);
  void draw();
  void drawScore(int score);
  void drawTimer(int time);
  void clearTimer();
  void reset();
};

class Modal{
    WINDOW *win;
    public:
    Modal(WINDOW *WIN);
    void draw();
    void drawTitle(const char* title);
    void drawScore(int score);
    void drawTimer(int time);
    void drawOptions();
    void clear();
};