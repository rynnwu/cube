#include "cube.h"

#include <cstdint>
#include <iostream>

using namespace std;

// A pixel point, includes R/G/B channel data.
struct PixelPoint {
    uint16_t b;
    uint16_t g;
    uint16_t r;
} __attribute__((packed));


// typedef MyCube to store 17x17x17 PixelPoints, and buffer alignment is 16
typedef Cube<PixelPoint, 17> MyCube;

// example to access elements as fast as possible
// set all element (r,g,b)=(10,20,30)
void fastAccessExample1(MyCube* pCube) {
    PixelPoint* p = pCube->getData();
    for (int z = 0; z < 17; ++z)
        for (int y = 0; y < 17; ++y)
            for (int x = 0; x < 17; ++x) {
                p->r = 10;
                p->g = 20;
                p->b = 30;
                p++;
            }
}

// example to access PixelPoints by the primitive way
// set all element (r,g,b)=(125,250,500)
void fastAccessExample2(MyCube* pCube) {
    uint16_t* p16 = reinterpret_cast<uint16_t*>(pCube->getBuffer());
    for (int z = 0; z < 17; ++z)
        for (int y = 0; y < 17; ++y)
            for (int x = 0; x < 17; ++x) {
                *p16 = 500;  // b
                p16++;
                *p16 = 250;  // g
                p16++;
                *p16 = 125;  // r
                p16++;
            }
}

int main()
{
    MyCube c3dCube0;

    cout << "element size = " << MyCube::TypeSize << endl;
    cout << "element count = " << MyCube::ElementCount << endl;
    cout << "buffer size (aligned) = " << c3dCube0._bufferSize << endl;

    fastAccessExample1(&c3dCube0);
    cout << "ex1: rgb data at (1,0,5) = "
        << c3dCube0(1, 0, 5).r << ", "
        << c3dCube0(1, 0, 5).g << ", "
        << c3dCube0(1, 0, 5).b << endl;

    fastAccessExample2(&c3dCube0);
    cout << "ex2: rgb data at (1,0,5) = "
        << c3dCube0(1, 0, 5).r << ", "
        << c3dCube0(1, 0, 5).g << ", "
        << c3dCube0(1, 0, 5).b << endl;


    return 0;
}

