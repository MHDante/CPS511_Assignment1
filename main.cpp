#include "Game.h"
#include "GLApp.h"

int main(int argc, char** argv)
{
  srand(unsigned(time(nullptr)));
  randZeroToOne();
  auto res = SetWindowPos(GetConsoleWindow(), HWND_TOP, 0, 500, 900, 200, SWP_SHOWWINDOW);
  res += 5;
  Game app = Game();
  app.init(argc, argv);
  
}
