#pragma once
#include <ctime>

class Tick{
    double deltaTime;
    clock_t tick1,tick2;
    double tickAdder;
    public:
    Tick();
    void captureTick1();
    void captureTick2();
    bool tickUpdate();
    bool frameUpdate();
    bool update();
    double getDeltaTime();
    void calculateDeltaTime();
};