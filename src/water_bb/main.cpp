#include <gameEngine/game.h>

void generate(Stage * stage)
{

}

int main()
{
  int w = 1920, h = 1080;
  Game game(w,h,"Super Game");

  game.run(generate);
  
  return 0;
}
