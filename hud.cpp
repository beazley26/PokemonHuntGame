#include "hud.h"
#include "clock.h"
#include <cmath>
#include <string>
#include <stdio.h>

Hud& Hud::getInstance() {
  if( SDL_WasInit(SDL_INIT_VIDEO)==0) {
     throw std::string("Must init SDL before HUD");
  }
  static Hud instance;
  return instance;
}

Hud::Hud() :
   io( IOManager::getInstance() )
{ }

void Hud::display(int num,int score,int freelist,int active) {
   SDL_Surface * const screen = io.getScreen();
   const Uint32 RED = SDL_MapRGB(screen->format,0xff,0,0);
   //const Uint32 WHITE = SDL_MapRGB(screen->format,0xff,0xff,0xff);
   
   // draw HUD frame
   Sint16 x1 = 0, x2 = 200;
   Sint16 y1 = 0, y2 = 220;
   //Draw_AALine(screen,x2/2,y1,x2/2,y2,200.0f,WHITE);
   Draw_AALine(screen,x1,y1,x1,y2,3.0f,RED);
   Draw_AALine(screen,x2,y1,x2,y2,3.0f,RED);
   Draw_AALine(screen,x1,y1,x2,y1,3.0f,RED);
   Draw_AALine(screen,x1,y2,x2,y2,3.0f,RED);
   
   // display HUD text
   Clock::getInstance().display();
   // score
   io.printMessageValueAt("Score: ",score,10,45);
   io.printMessageValueAt("Pokeballs: ",num,10,60);
   io.printMessageValueAt("Free: ",freelist,10,75);
   io.printMessageValueAt("Active: ",active,10,90);
   // controls
   io.printMessageAt("W - UP",10,110);
   io.printMessageAt("A - LEFT",10,125);
   io.printMessageAt("D - RIGHT",10,140);
   io.printMessageAt("S - DOWN",10,155);
   io.printMessageAt("J - THROW POKEBALL",10,170);
   io.printMessageAt("K - SUICIDE",10,185);
   io.printMessageAt("R - RESET", 10, 200);
}


   
