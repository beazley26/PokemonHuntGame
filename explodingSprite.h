#ifndef EXPLODINGSPRITE_H
#define EXPLODINGSPRITE_H
#include <vector>
#include <list>
#include "sprite.h"
#include "chunk.h"

class ExplodingSprite : public Sprite {
public:
  ExplodingSprite(const Sprite& s);
  ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
private:
  std::list<Chunk> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk> freeList; // When a chunk gets out of range it goes here

  std::vector<SDL_Surface*> surfaces; // Each chunk has a Surface
  std::vector<Frame*> cframes; // Each chunk has a Frame

  ExplodingSprite(const ExplodingSprite&); // Explicit disallow (Item 6)
  ExplodingSprite& operator=(const ExplodingSprite&); // (Item 6)
};
#endif
