#include "Modeller.h"
#include "GLApp.h"
#include <windows.h>

int main(int argc, char** argv)
{
  auto res = SetWindowPos(GetConsoleWindow(), HWND_TOP, 0, 500, 900, 200, SWP_SHOWWINDOW);
  res += 5;
  Modeller app = Modeller();
  app.init(argc, argv);
  
}
