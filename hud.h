#ifndef HUD_H
#define HUD_H
#include "aaline.h"
#include "ioManager.h"
#include "gamedata.h"
#include <SDL.h>

class Hud {
public:
  static Hud& getInstance();
  void display(int num,int score,int freelist,int active);
private:
  const IOManager& io;
  
  Hud();
  Hud(const Hud&);
  Hud&operator=(const Hud&);
};
#endif
