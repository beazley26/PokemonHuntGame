#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),
  explosion(NULL),
  frames( FrameFactory::getInstance().getFrames(name) ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames(Gamedata::getInstance().getXmlInt(name+"/frames")),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame(0),
  moving(false),
  stoping(false),
  exploded(false),
  movePosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")))
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel), 
  explosion(NULL),
  frames( FrameFactory::getInstance().getFrames(n) ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames(Gamedata::getInstance().getXmlInt(n+"/frames")),
  frameInterval( Gamedata::getInstance().getXmlInt(n+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  moving(false),
  stoping(false),
  exploded(false),
  movePosition(pos)
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel, Frame* f):
  Drawable(n,pos,vel),
  explosion(NULL),
  frames(),
  frameWidth(f->getWidth()),
  frameHeight(f->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames(1),
  frameInterval(0),
  timeSinceLastFrame(0),
  moving(false),
  stoping(false),
  exploded(false),
  movePosition(pos)
  {
	frames.push_back(f);  
  } 

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel, std::vector<Frame*> f):
  Drawable(n, pos, vel), 
  explosion(NULL),
  frames( f ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames(Gamedata::getInstance().getXmlInt(n+"/frames")),
  frameInterval( Gamedata::getInstance().getXmlInt(n+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  moving(false),
  stoping(false),
  exploded(false),
  movePosition(pos)
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s), 
  explosion(s.explosion),
  frames(s.frames),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  moving(s.moving),
  stoping(s.stoping),
  exploded(s.exploded),
  movePosition(s.movePosition)
{ }

Sprite& Sprite::operator=( const Sprite& s) {
  Drawable::operator=(s);
  explosion=s.explosion;
  frames=s.frames; 
  frameWidth=s.getFrame()->getWidth();
  frameHeight=s.getFrame()->getHeight();
  worldWidth=Gamedata::getInstance().getXmlInt("world/width");
  worldHeight=Gamedata::getInstance().getXmlInt("world/height");
  currentFrame=s.currentFrame;
  numberOfFrames=s.numberOfFrames;
  frameInterval=s.frameInterval;
  timeSinceLastFrame=s.timeSinceLastFrame;
  moving=s.moving;
  stoping=s.stoping;
  exploded=s.exploded;
  movePosition=s.movePosition;
  return *this;
} 

void Sprite::draw() const {
  if(explosion) {
    explosion->draw();
    return;
  } 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y); 
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::explode() {
  if(explosion) {
    return;
  }
  explosion=new ExplodingSprite(*this);
}

void Sprite::update(Uint32 ticks) { 
  if(explosion) {
    explosion->update(ticks);
    if(explosion->chunkCount()==0) {
      exploded=true;
      delete explosion;
      explosion=NULL;
    }
    return;
  }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
}

void Sprite::moveUp() {
  if(!moving && !stoping) {
   movePosition=getPosition();
   velocityY(-200);
   moving=true;
  }
}

void Sprite::moveDown() {
  if(!moving && !stoping) {
   movePosition=getPosition();
   velocityY(200);
   moving=true;
  }
}

void Sprite::moveLeft() {
  if(!moving && !stoping) {
   movePosition=getPosition();
   velocityX(-200);
   moving=true;
  }
}

void Sprite::moveRight() {
  if(!moving && !stoping) {
   movePosition=getPosition();
   velocityX(200);
   moving=true;
  }
}

void Sprite::stop() {
  if(moving) {
    stoping=true;
    moving=false;
  }
}

