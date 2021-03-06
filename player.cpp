#include <cmath>
#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"

void Player::advanceFrame(Uint32 ticks) {
   static int downStat = Gamedata::getInstance().getXmlInt(getName()+"/downFrame");
   static int upStat = Gamedata::getInstance().getXmlInt(getName()+"/upFrame");
   static int leftStat = Gamedata::getInstance().getXmlInt(getName()+"/leftFrame");
   static int rightStat = Gamedata::getInstance().getXmlInt(getName()+"/rightFrame");
   static int pD = Gamedata::getInstance().getXmlInt(getName()+"/FramesperDirection");
   
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
	  if(velocityX()>0) {
	     direction = 3;
	  	  currentFrame = ((currentFrame+1)%pD) + (numberOfFrames/2);
	  }
	  else if(velocityX()<0) {
	     direction = 1;
	     currentFrame = ((currentFrame+1)%pD) + (numberOfFrames/4);
	  }
	  else if(velocityY()>0) {
	     direction = 0;
	     currentFrame = ((currentFrame+1)%pD);
	  }
	  else if(velocityY()<0) {
	     direction = 2;
	     currentFrame = ((currentFrame+1)%pD) + (numberOfFrames*3/4);
	  } 
	  else {
	     if(currentFrame < numberOfFrames/4) {
	       currentFrame = downStat;
	     } 
	     else if(currentFrame < numberOfFrames/2) {
	       currentFrame = leftStat;
	     }
	     else if(currentFrame < numberOfFrames*3/4) {
	       currentFrame = rightStat;
	     }
	     else {
	       currentFrame = upStat;
	     }
	  }
   timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  Sprite(name),
  direction(0),
  balls(Gamedata::getInstance().getXmlInt(name+"/pokeballs")),
  score(0),
  thrown(),
  freeammo(),
  strategy(NULL)
  //poke()
{ 
  strategy=new PerPixelCollisionStrategy;
  //initialize();
}

Player::Player(const Player& s) :
  Sprite(s),
  direction(s.direction),
  balls(s.balls),
  score(s.score),
  thrown(s.thrown),
  freeammo(s.freeammo),
  strategy(s.strategy)
  //poke(s.poke)
  { }
  
//Player::~Player() {
//
//}

/*  
void Player::initialize() {
  int pokenum = Gamedata::getInstance().getXmlInt("PokemonNum");
  for(int i=0;i<pokenum;++i) {
    Pokemon p("temp");
    p.X(rand()%(Gamedata::getInstance().getXmlInt("world/width")));
    p.Y(rand()%(Gamedata::getInstance().getXmlInt("world/height")));
    poke.push_back(p);
  }
} */

void Player::draw() const { 
  if(explosion) {
    explosion->draw();
    return;
  } 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y); 
  
  std::list<Sprite>::const_iterator ptr=thrown.begin();
  while(ptr!=thrown.end()) {
	ptr->draw();
    ++ptr;	
  }
  /*
  std::list<Pokemon>::const_iterator ptr2=poke.begin();
  while(ptr2!=poke.end()) {
   ptr2->draw();
   ++ptr2;
  } */
}

void Player::update(Uint32 ticks) { 
  if(explosion) {
    explosion->update(ticks);
    if(explosion->chunkCount()==0) {
      delete explosion;
      explosion=NULL;
    }
    return;
  }

  advanceFrame(ticks);

  //Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  //setPosition(getPosition() + incr);

  if ( Y() < 0 && velocityY() < 0) {
    velocityY(0);
    moving=false;
    stoping=false;
  }
  if ( (Y() >= worldHeight - frameHeight) && velocityY() > 0) {
    velocityY(0);
    moving=false;
    stoping=false;
  }

  if ( X() < 0 && velocityX() < 0) {
    velocityX(0);
    moving=false;
    stoping=false;
  }
  if ( (X() >= worldWidth - frameWidth) && velocityX() > 0) {
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

  // thrown pokeballs
  std::list<Sprite>::iterator ptr=thrown.begin();
  while(ptr!=thrown.end()) {
    ptr->update(ticks);
	 if(hypot(X()-ptr->X(),Y()-ptr->Y())>200) {
		 freeammo.push_back(*ptr);
		 ptr=thrown.erase(ptr);
    } else {
		 ++ptr;
	 }
  } 
  /*
  std::list<Pokemon>::iterator ptr2=poke.begin();
  while(ptr2!=poke.end()) {
    ptr2->update(ticks,getPosition());
    if(gotCatch(*ptr2)) {
      ptr2->explode();
      ++ptr2;
    }
    else if(ptr2->hasBlown()==true) {
      delete (&ptr2);
      ptr2=poke.erase(ptr2);
    }
    else {
      ++ptr2;
    }
  }
  
  int size=poke.size();
  int pokenum = Gamedata::getInstance().getXmlInt("PokemonNum");
  for(int i=0;i<(pokenum-size);++i) {
    Pokemon p("temp");
    p.X(rand()%(Gamedata::getInstance().getXmlInt("world/width")));
    p.Y(rand()%(Gamedata::getInstance().getXmlInt("world/height")));
    poke.push_back(p);
  }*/
}


void Player::throwBall() {
  if(balls>0) {
   int i=freeammo.size();
   if(i==0) {
	  Sprite s("ball");
	  freeammo.push_back(s);
   }
   std::list<Sprite>::iterator ptr=freeammo.begin();
   ptr->setPosition(getPosition());

   if(direction==0){ // down
     ptr->velocityX(0);
     ptr->velocityY(200);
   }
   if(direction==1) {
     ptr->velocityX(-200);
     ptr->velocityY(0);
   }
   if(direction==2) {
    ptr->velocityX(0);
    ptr->velocityY(-200);
   }
   if(direction==3) {
     ptr->velocityX(200);
     ptr->velocityY(0);
   }
   thrown.push_back(*ptr);
   ptr=freeammo.erase(ptr);
   --balls;
  }
}

void Player::reset() {
   std::string temp=getName();
   setPosition(Vector2f(Gamedata::getInstance().getXmlInt(temp+"/startLoc/x"),
               Gamedata::getInstance().getXmlInt(temp+"/startLoc/y")));
   setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(temp+"/speedX"),
               Gamedata::getInstance().getXmlInt(temp+"/speedY")));
   currentFrame=1;
   timeSinceLastFrame=0;
   freeammo.clear();
   thrown.clear();
   balls=Gamedata::getInstance().getXmlInt(temp+"/pokeballs");
   score=0;
   direction=0;
}

bool Player::gotCatch(Pokemon& d) {
  std::list<Sprite>::iterator ptr=thrown.begin();
  while(ptr!=thrown.end()) {
    if(strategy->execute(*ptr,d)) {
      freeammo.push_back(*ptr);
      ptr=thrown.erase(ptr);
      score+=d.getPoints();
      return true;
    }
    ++ptr;
  }
  return false;
}
