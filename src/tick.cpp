#include "../header/tick.hpp"
#include "../header/settings.hpp"
#include <cmath>
#include <stdexcept>
#include <unistd.h>


Tick::Tick() {
  deltaTime = 0.0;
  tick1 = tick2 = 0;
  tickAdder = 0;
};

void Tick::captureTick1() {
  tick1 = clock();
  if (tick1 == -1) {
    throw std::runtime_error("An error occurred in using clock()");
  }
};

void Tick::captureTick2() {
  tick2 = clock();
  if (tick2 == -1) {
    throw std::runtime_error("An error occurred in using clock()");
  }
}

void Tick::calculateDeltaTime() {
  deltaTime = (double)(tick2 - tick1) / CLOCKS_PER_SEC;
}

double Tick::getDeltaTime(){
    return deltaTime;
}


bool Tick::update(){
    
    return true;
}

bool Tick::tickUpdate(){
  double await = ((double)(1.0/TICK_RATE) - deltaTime);
  if(await > 0){
    usleep(await * 1e6);
  }
    tickAdder+=await;
    return true;
}

bool Tick::frameUpdate(){
    if(tickAdder >= (double)(1.0/FRAME_RATE)){
      tickAdder = fmod(tickAdder, (double)(1.0/FRAME_RATE));
      return true;
    }
    return false;
}