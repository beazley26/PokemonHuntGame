#include <iostream>
#include <string>
#include <iomanip>
#include "gamedata.h"
#include "manager.h"
#include "player.h"
#include "hud.h"

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  //for (unsigned i = 0; i < sprites.size(); ++i) {
  //  delete sprites[i];
  //}
  
  for (unsigned j=0; j<players.size(); ++j) {
    delete players[j];
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  hud( Hud::getInstance() ),
  screen( io.getScreen() ),
  world1("back1", Gamedata::getInstance().getXmlInt("back1/factor") ),
  world2("back2", Gamedata::getInstance().getXmlInt("back2/factor") ),
  viewport( Viewport::getInstance() ),
 
  players(),
  currentPlayer(0),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  showHUD(true)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  //makeSprites();
  players.push_back( new Player("character") );
  viewport.setObjectToTrack(players[0]);
}

void Manager::draw() const {
  world1.draw();
  world2.draw();
  //for (unsigned i = 0; i < sprites.size(); ++i) {
  //  sprites[i]->draw();
  //}
  
  players[currentPlayer]->draw();

  if(showHUD) {
    hud.display(players[currentPlayer]->Pokeballs(),players[currentPlayer]->getScore(),
                players[currentPlayer]->freelist(),players[currentPlayer]->active());
  }
  io.printMessageAt(title, 10, 450);
  viewport.draw();

  SDL_Flip(screen);
}

// create "Pokemon" sprites
/*void Manager::makeSprites() {
  unsigned spriteNum=Gamedata::getInstance().getXmlInt("PokemonNum");
  sprites.reserve(spriteNum);
  for(unsigned i=0;i<spriteNum;++i) {
    sprites.push_back( new Sprite("temp"));
    sprites[i]->X(rand()%(Gamedata::getInstance().getXmlInt("world/width")));
    sprites[i]->Y(rand()%(Gamedata::getInstance().getXmlInt("world/height"))); 
  }
} */

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

//void Manager::switchPlayer() {
//  currentPlayer = (currentPlayer+1) % players.size();
//  viewport.setObjectToTrack(players[currentPlayer]);
//}

void Manager::update() {
  ++(clock);
  Uint32 ticks = clock.getElapsedTicks();

  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
  }
  
  // turn off HUD after 2 seconds
  if (clock.getSeconds() == 2) {
    showHUD = false;
  }

  players[currentPlayer]->update(ticks);
  
  /*for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
    if(players[currentPlayer]->gotCatch(sprites[i])) {
      sprites[i]->explode();
    }
  } */
  
  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  world1.update();
  world2.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if (event.type == SDL_KEYUP) {
        players[currentPlayer]->stop();
      }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        // toggle HUD
        if ( keystate[SDLK_F1] ) {
          if(showHUD == true) {
            showHUD = false;
          }
          else {
            showHUD = true;
          }
        }
        //if ( keystate[SDLK_t] ) {
        //  switchPlayer();
        //}
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (keystate[SDLK_a]) {
          players[currentPlayer]->moveLeft();
        }
        if (keystate[SDLK_d]) {
          players[currentPlayer]->moveRight();        
        }
        if (keystate[SDLK_s]) {
          players[currentPlayer]->moveDown();        
        }
        if (keystate[SDLK_w]) {
          players[currentPlayer]->moveUp();        
        }
        if (keystate[SDLK_j]) {
          players[currentPlayer]->throwBall();
        }
        if (keystate[SDLK_k]) { // kill player
          players[currentPlayer]->explode();
        }
        if (keystate[SDLK_r]) { // reset
          players[currentPlayer]->reset();      
        }
      }
    }
    draw();
    update();
  }
}
