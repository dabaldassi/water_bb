#include <gameEngine/game.h>
#include <gameEngine/actor/controlable/controlable.h>

#define WIDTH 1920
#define HEIGHT 1080

void generate(Stage * stage)
{
  constexpr int NB_BLOCK_W = 21;
  constexpr int NB_BLOCK_H = 8;
  constexpr float SIZE_BLOCK_W = WIDTH/NB_BLOCK_W;
  constexpr float SIZE_BLOCK_H = HEIGHT/NB_BLOCK_H;
  
}

int main()
{
  Game game(WIDTH,HEIGHT,"Water Boat Battle");
  
  game.run(generate);
  
  return 0;
}
