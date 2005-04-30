#include "Gil.h"

using namespace Gil;

int main()
{
    Image<unsigned char, ThreeChannels> a(200, 100);
    for (int y = 0; y < a.height(); ++y) {
	for (int x = 0; x < a.width(); ++x) {
	    a[x][y] = Pixel<unsigned char, ThreeChannels>(128, 128, 128);
	    for (int c = 0; c < a.channels(); ++c) {
		a[x][y][c] = 128;
	    }
	}
    }
    return 0;
}
