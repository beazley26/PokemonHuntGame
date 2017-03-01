#include <cmath>
#include "pokemon.h"
#include "gamedata.h"
#include "frameFactory.h"

Pokemon::Pokemon(const std::string& name) :
   Sprite(name),
   power(Gamedata::getInstance().getXmlInt(name+"/power")),
   runDist(Gamedata::getInstance().getXmlInt(name+"/flee"))
{ }

Pokemon::Pokemon(const Pokemon& pika) :
   Sprite(pika),
   power(pika.power),
   runDist(pika.runDist)
{ }

//Pokemon& Pokemon::operator=(const Pokemon& p) {
//   Sprite::operator=(p);
//   power=p.power;
//   runDist=p.runDist;
//   return *this;
//}

void Pokemon::draw() const {
   if(explosion) {
      explosion->draw();
      return;
   }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y); 
}

void Pokemon::update(Uint32 ticks,const Vector2f& playerPos) {
  if(explosion) {
    explosion->update(ticks);
    if(explosion->chunkCount()==0) {
      delete explosion;
      explosion=NULL;
      exploded=true;
    }
    return;
  }
  
  if ( Y() < 0) {
    velocityY(0);
    moving=false;
    stoping=false;
  }
  if ( Y() >= worldHeight - frameHeight) {
    velocityY(0);
    moving=false;
    stoping=false;
  }

  if ( X() < 0) {
    velocityX(0);
    moving=false;
    stoping=false;
  }
  if ( X() >= worldWidth - frameWidth) {
    velocityX(0);
    moving=false;
    stoping=false;
  }
  
  int dist = (int)(hypot(X()-movePosition[0],Y()-movePosition[1]));
  if(dist%32 == 0 && stoping) {
   if(velocityX()>0 || velocityX()<0) {
      velocityX(0);
   }
   if(velocityY()>0 || velocityY()<0) {
      velocityY(0);
   }
   stoping=false;
  }
  
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  
  int distX = abs(X()-playerPos[0]);
  int distY = abs(Y()-playerPos[1]);
  
  if(distX < runDist || distY < runDist) {
   if(X()>playerPos[0]) { moveRight(); }
   if(X()<playerPos[0]) { moveLeft(); }
   if(Y()>playerPos[1]) { moveDown(); }
   if(Y()<playerPos[1]) { moveUp(); }
  }
}

void Pokemon::moveRandom() {
  int x = 10000/power;
  if(rand()%x == 1) { moveLeft(); }
  if(rand()%x == 2) { moveRight(); }
  if(rand()%x == 3) { moveUp(); }
  if(rand()%x == 4) { moveDown(); }
}
