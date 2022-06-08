#include <Magick++.h>
#include <iostream>

using namespace Magick;

int main(int argc, char *argv[]) {
  InitializeMagick(*argv);

  ColorHSL bg = ColorHSL(0.61, 0.16, 0.22);
  Image frame = Image("500x300", bg);

  std::cout << "Test" << std::endl;

  return 0;
}
