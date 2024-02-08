#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstring>
#include "header/OctreeColor.hpp"
#include "header/Color.hpp"
// #include "emscripten.h"

 #define EXTERN_C extern "C"

using namespace std;

int main(int argc, char ** argv) {
  Octree* ddd = new Octree();

  vector<RGB*> ss = BuildColorRGB();
  ddd->buildOctree(ss,32);
  
  vector<ColorCount*> ssss;
  ddd->colorStats(ddd->root,ssss);

  for (size_t i = 0; i < ssss.size(); i++)
  {
    // printf("%d\n", ssss[i]->color[0]);
    // printf("%d\n", ssss[i]->color[1]);
    // printf("%d\n", ssss[i]->color[2]);
    cout << ssss[i]->color[0] << "," << ssss[i]->color[1] << "," << ssss[i]->color[2] << endl;
  }

  return 0;
}

// EXTERN_C EMSCRIPTEN_KEEPALIVE 
// color_t* myFunction(color_t *data, int len, int maxColor) {
//     Octree* ddd = new Octree();

//     vector<RGB*> rgb;

//     printf("像素\n");

//     for(size_t i =0;i < len;(i += 4)){
//         RGB* tmp = new RGB(data[i],data[i+1],data[i+2]);
//         rgb.push_back(tmp);
//     }

//     printf("hahah%d\n", data[1]);

//     // vector<RGB*> ss = ddd->buildColorRGB(rgb);
//     ddd->buildOctree(rgb,maxColor);
    
//     vector<ColorCount*> colors;
//     ddd->colorStats(ddd->root,colors);

//     // color_t* result = (color_t*)malloc((maxColor + 1)* 3 * 4);

//     data[0] = (color_t)colors.size();

//     for(int i = 1;i <= colors.size();i++){
//         data[(1 + (i - 1) * 3)] = colors[(i - 1)]->red;
//         data[(1 + (i - 1) * 3) + 1] = colors[(i - 1)]->green;
//         data[(1 + (i - 1) * 3) + 2] = colors[(i - 1)]->blue;
//     }

//     return data;
// }

// EXTERN_C EMSCRIPTEN_KEEPALIVE
// int add(int a,int b) {
// 	return (a+b);
// }

// g++ `pkg-config opencv4 --libs --cflags opencv4` ./main.cpp -o ./main.o -std=c++11
// ./main.o 