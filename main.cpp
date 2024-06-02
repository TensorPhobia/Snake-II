#include "header/collision.hpp"
#include "header/control.hpp"
#include "header/gameObj.hpp"
#include "header/settings.hpp"
#include "header/tick.hpp"
#include "header/ui.hpp"
#include <cctype>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <curses.h>
#include <panel.h>
#include <unistd.h>

int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
#pragma region Curses_Init
  initscr();
  cbreak(); // Disable line buffering
  noecho(); // Disable echoing of characters
  curs_set(
      0); // Set cursor visibility (0: invisible, 1: normal, 2: very visible)
  nodelay(stdscr, TRUE); // Enable non-blocking input
  timeout(0);            // Set getch to return immediately if there's no input
  keypad(stdscr, true);
  start_color();
  use_default_colors();
  init_pair(1, COLOR_CYAN, 0);
  init_pair(2, COLOR_RED, 0);
  init_pair(3, COLOR_YELLOW, 0);
  init_pair(4, COLOR_GREEN, 0);
  init_pair(5, COLOR_BLUE, 0);
#pragma endregion Curses_Init

#pragma region Partion_Screen
  int screenMaxY, screenMaxX;

  getmaxyx(stdscr, screenMaxY, screenMaxX);
  WINDOW *winScore = newwin(2, screenMaxX, 0, 0);
  WINDOW *winPlayGround = newwin(screenMaxY - 2, screenMaxX, 2, 0);
  WINDOW *winMenu = newwin(screenMaxY - 2, screenMaxX / 4, 1,
                           (screenMaxX - screenMaxX / 4) / 2); // testing
  PANEL *overlays[3];
  overlays[0] = new_panel(winMenu);
  overlays[1] = new_panel(winPlayGround);
  overlays[2] = new_panel(winScore);
#pragma endregion Partion_Screen

#pragma region Set_Boundary
  bool bounded = false;
  if (argc == 2 &&
      (strcmp(argv[1], "bounded") == 0 || strcmp(argv[1], "-b") == 0)) {
    bounded = true;
    wattron(winPlayGround, COLOR_PAIR(5));
    box(winPlayGround, 0, 0);
    wattroff(winPlayGround, COLOR_PAIR(5));
  }

  wborder(winScore, ' ', ' ', ' ', ACS_HLINE, ' ', ' ', ' ', ' ');

  refresh();
  wrefresh(winPlayGround);
#pragma endregion Set_Boundary

#pragma region Game_Logic

  auto tick = Tick();
  auto spider = Spider(winPlayGround);
  auto food = Food(winPlayGround);
  auto snake = Snake(winPlayGround);
  auto titleBar = TitleBar(winScore);
  auto modal = Modal(winMenu);
  int gametick = 0;
  int registerdKey;
  unsigned long int seconds = 0;
  unsigned long int secondCaptured = 0;
  unsigned int score = 0;
  unsigned int eatCount = 0;
  short coolDown = 0; /*per tick*/
  int snakeDirection, lastSnakeDirection;
  snakeDirection = ERR;
  lastSnakeDirection = KEY_RIGHT;
  bool runing = true;

  while (runing) {
  restart:;
    tick.captureTick1();
#pragma region Tick_Update
    if (tick.tickUpdate()) {
      /*Handle in game timer*/
      if (++gametick >= TICK_RATE) {
        gametick = 0;
        seconds++;
      }
      registerdKey = getch();
      snakeDirection = isArrow(registerdKey) ? registerdKey : snakeDirection;
    }
#pragma endregion Tick_Update
#pragma region Frame_Update
    if (tick.frameUpdate()) {
      /*handle snake movement*/
      if (isLegalMove(snakeDirection, lastSnakeDirection)) {
        lastSnakeDirection = snakeDirection;
      } else {
        snakeDirection = lastSnakeDirection;
      }
      switch (snakeDirection) {
      case KEY_UP: {
        snake.moveUp();
        break;
      }
      case KEY_DOWN: {
        snake.moveDown();
        break;
      }
      case KEY_LEFT: {
        snake.moveLeft();
        break;
      }
      case KEY_RIGHT: {
        snake.moveRight();
        break;
      }
      }

      /*Collision detection*/
      if (snakeCollidedWithFood(snake.cursor, food.pos)) {
        score++;
        titleBar.drawScore(score);
        eatCount++;
        if (eatCount % SPIDER_SPAN_RATE == 0 && !spider.isAlive) {
          spider.randomizeSprite();
          spider.setIsAlive(true);
          spider.pickEmptySpot();
          spider.draw();
          secondCaptured = seconds;
          titleBar.drawTimer(seconds);
        }
        food.pickEmptySpot();
        food.draw();
      } else {
        if (coolDown <= 0) {
          snake.pop_front();
        } else {
          coolDown--;
        }
        if (spider.isAlive && snakeCollidedWithSpider(snake, spider)) {
          score += 10;
          titleBar.drawScore(score);
          spider.clear();
          spider.setIsAlive(false);
          titleBar.clearTimer();
          coolDown = 10;
        } else if (snakeCollidedWithSnake(snake) ||
                   (bounded && snakeCollidedWithBoundary(snake)) || score >= MAX_SCORE) {
          // handle death
          modal.clear();
          snakeDirection = lastSnakeDirection = ERR;
          //runing = false;






#pragma region GameOver_Screen
          if(score>=MAX_SCORE){
            wattron(winMenu,COLOR_PAIR(3));
            modal.drawTitle("You Win!");
            wattroff(winMenu,COLOR_PAIR(3));
          }else{
            modal.drawTitle("Game Over");
          }
          modal.drawScore(score);
          modal.drawTimer(seconds);
          modal.drawOptions();
          box(winMenu, 0, 0);
          show_panel(overlays[0]);
          update_panels();
          timeout(-1);
          doupdate();
          do{
            registerdKey = toupper(getch());
            switch (registerdKey) {
            case 'R': {
              seconds = 0;
              secondCaptured = 0;
              score = 0;
              eatCount = 0;
              coolDown = 0; /*per tick*/
              timeout(0);
              hide_panel(overlays[0]);
              update_panels();
              doupdate();
              titleBar.clearTimer();
              titleBar.reset();
              spider.reset();
              food.reset();
              snake.reset();
              goto restart;
              break;
            }
            case 'Q': {
              runing = false;
              goto exit;
            }
            }
          }while(true);

#pragma endregion GameOver_Screen

        }
      }
      /*Snake should be drawn after spider gets cleared from screen
        so no common cells will be cleared un intentionally
      */
      snake.push_back(snake.cursor);
      /*handle spider life span*/
      if (spider.isAlive) {
        if (seconds - secondCaptured >= SPIDER_LIFE_SPAN) {
          spider.setIsAlive(false);
          spider.clear();
          titleBar.clearTimer();
        } else {
          titleBar.drawTimer(SPIDER_LIFE_SPAN - seconds + secondCaptured);
        }
      }
    }
#pragma endregion Frame_Update
    tick.captureTick2();
    tick.calculateDeltaTime();
  }
#pragma endregion Game_Logic
exit:;

#pragma region Curses_Delete
  endwin();
  return EXIT_SUCCESS;
#pragma endregion Curses_Delete
}