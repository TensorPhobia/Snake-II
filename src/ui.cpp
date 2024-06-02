#include "../header/settings.hpp"
#include "../header/ui.hpp"
#include <curses.h>
#include <string.h>

#pragma region Title_Bar

TitleBar::TitleBar(WINDOW *WIN){
    win = WIN;
    wbkgd(win, COLOR_PAIR(2));
    draw();
    drawScore(0);
}

void TitleBar::draw(){
    const char* title = "Snake II";
    mvwaddstr(win, 0, (getmaxx(win) - strlen(title))/2, title);
    wrefresh(win);
};

void TitleBar::drawScore(int score){
    char text[1000];
    sprintf(text, " score: %d ", score);
    mvwaddstr(win, 0, 0, text);
    wrefresh(win);
};

void TitleBar::drawTimer(int time){
    char text[100];
    sprintf(text, " spider: %d ", time);
    mvwaddstr(win, 0, getmaxx(win) - strlen(text), text);
    wrefresh(win);
}

void TitleBar::clearTimer(){
    char text[100];
    sprintf(text, " spider: %d ", SPIDER_LIFE_SPAN);
    for (int i = 0; text[i] != '\0'; i++) {
        mvwaddch(win, 0, getmaxx(win) - i, ' ');
    }
    wrefresh(win);
}

void TitleBar::reset(){
    clearTimer();
    char text[100];
    sprintf(text, " score: %d ", MAX_SCORE);
    for (int i = 0; text[i] != '\0'; i++) {
        mvwaddch(win, 0,i, ' ');
    }
    wrefresh(win);
    drawScore(0);
}


#pragma endregion Title_Bar

#pragma region Death_Screen
Modal::Modal(WINDOW *WIN){
    win = WIN;
    wbkgd(win, COLOR_PAIR(1));
}

void Modal::drawTitle(const char* title){
    mvwaddstr(win, 1, (getmaxx(win) - strlen(title))/2, title);
    wrefresh(win);
}

void Modal::drawScore(int score){
    char text[1000];
    sprintf(text, " score: %d ", score);
    mvwaddstr(win, getmaxy(win)/4, 2, text);
    wrefresh(win);
}

void Modal::drawTimer(int time){
    char text[1000];
    sprintf(text, "time: %d sec", time);
    mvwaddstr(win, getmaxy(win)/3, 3, text);
    wrefresh(win);
}

void Modal::drawOptions(){
    char text1[1000] = "[Q] Exit";
    mvwaddstr(win, getmaxy(win)/2, 3, text1);
    char text2[1000] = "[R] Restart";
    mvwaddstr(win, getmaxy(win)/2, getmaxx(win)-3 - strlen(text2), text2);
    wrefresh(win);
}

void Modal::clear(){
    for (int i = 1; i < getmaxx(win)-1; i++) {
        mvwaddch(win, 1,i, ' ');
    }
    wrefresh(win);
}


#pragma endregion Death_Screen