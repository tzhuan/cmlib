#include <iostream>
#include "gil/gil.h"

using namespace std;
using namespace gil;

int main(int argc, char **argv)
{
  if (argc != 3) {
    cerr << "usage: " << argv[0] << " [input-file] [output-file]" << endl;
    return 1;
  }

  const char *fin = argv[1];
  const char *fout = argv[2];

  try {
    FloatImage4 image;

    if (!read(image, fin)) {
      cerr << "read error" << endl;
      return 1;
    }
    write(image, fout);
  } catch (exception &e) {
    cerr << e.what() << endl;
    return 1;
  }

  return 0;
}