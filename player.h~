#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include <list>
#include "drawable.h"
#include "sprite.h"
#include "explodingSprite.h"
#include "collisionStrategy.h"
#include "pokemon.h"

class Player : public Sprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  //void initialize();
  int Pokeballs() {return balls;}
  int getScore() {return score;}
  int freelist() {return freeammo.size();}
  int active() {return thrown.size();}
  void throwBall();
  void reset();
  bool gotCatch(Pokemon&);

protected:
  int direction;
  int balls;
  int score;
  std::list<Sprite> thrown;
  std::list<Sprite> freeammo;
  CollisionStrategy *strategy;
  //std::list<Pokemon> poke;

  void advanceFrame(Uint32 ticks);
  Player& operator=(const Player&);
};
#endif
