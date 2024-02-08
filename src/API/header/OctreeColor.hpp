#pragma once
#include <iostream>
// #include "opencv2/opencv.hpp"
// #include "emscripten.h"
#include <vector>
#include <bitset>
#include <list>
#include <algorithm>
#include "Color.hpp"

using namespace std;
using namespace cv;

class ColorCount
{
    public:
        unsigned int count;
        color_t color[3];
        color_t red;
        color_t green;
        color_t blue;
        color_t colorValue;
        static bool cmp(ColorCount* a, ColorCount* b)
        {
            if(a->count > b->count) return true;
            if(a->count < b->count) return false;
            return a->color[0] < b->color[0];
        }
};

color_t _GetColors(RGB* color,color_t level){
    color_t result = ((((color->r >> (7 - level) ) & 1)<< 2) + (((color->g>>(7 - level))&1)<<1) + ((color->b>>(7 - level))&1));
    return result;
};

class ColorNode{
    private:
        bool _isLeaf; //是否是叶子结点
        unsigned int _pixelCount; //像素计数
        bool _isReduce;
        level_t _level;

        color_t _blue; //蓝通道
        color_t _green; //绿通道
        color_t _red; //红通道

        ColorNode* _reduce;

    public:
        vector<ColorNode*> children = vector<ColorNode*>(8,nullptr); //下一层

        ColorNode():_isLeaf(false),_pixelCount(0),_blue(0),_red(0),_green(0),_level(0),_isReduce(true)
        {
            _reduce = nullptr;
        }
        ColorNode(RGB* color, level_t level):_blue(color->b),_red(color->r),_green(color->g),_level(level),_isReduce(true)
        {
            _reduce = nullptr;
        }

        bool Leaf(){
            return _isLeaf;
        }

        bool Level(){
            return _level;
        }

        void setIsLeaf(bool is){
            _isLeaf = is;
        }
        void setIsReduce(bool is){
            _isReduce = is;
        }
        void setIsLevel(level_t l){
            _level = l;
        }
        void addRed(color_t r){
            _red += r;
        }
        void setRed(color_t r){
            _red = r;
        }
        void addGreen(color_t g){
            _green += g;
        }
        void setGreen(color_t g){
            _green = g;
        }
        void addBlue(color_t b){
            _blue += b;
        }
        void setBlue(color_t b){
            _blue = b;
        }
        void addPixelCount(unsigned int c){
            _pixelCount += c;
        }
        void setPixelCount(unsigned int c){
            _pixelCount = c;
        }

        color_t red(){
            return _red;
        }
        color_t green(){
            return _green;
        }
        color_t blue(){
            return _blue;
        }
        unsigned int pixelCount(){
            return _pixelCount;
        }

        void setReduce(ColorNode* ss){
            _reduce = ss;
        }
};

class Octree{
    public:
        ColorNode* root;
        unsigned long leafNums;
        list<ColorNode*> reducible[7];
        Octree():leafNums(0)
        {
            root = new ColorNode();
        }

        ColorNode* createNode(level_t level);

        void addColor(ColorNode* node, RGB* color, int level);
        void reduceTree();
        void buildOctree(vector<RGB*> &color, int maxColor);
        vector<RGB*> buildColorRGB();
        void colorStats(ColorNode* node, vector<ColorCount*> &colors);
};

ColorNode* Octree::createNode(level_t level){
    ColorNode* node = new ColorNode();

    if(level == 7){
        node->setIsLeaf(true);
        leafNums++;
    }else{
        reducible[level].push_front(node);
    }

    return node;
}

void Octree::addColor(ColorNode* node, RGB* color, int level){
    if(node->Leaf()) {
        node->addPixelCount(1);
        node->addRed(color->r);
        node->addGreen(color->g);
        node->addBlue(color->b);
    } else {
        int idx = _GetColors(color, level);

        if(nullptr == node->children[idx]) {
            node->children[idx] = createNode(level + 1);
        }

        addColor(node->children[idx], color, level + 1);
    }
}

void Octree::reduceTree(){
    level_t lv = 6;
    while (reducible[lv].empty()) lv--;
    
    ColorNode* node = reducible[lv].front();
    reducible[lv].pop_front();

    color_t r = 0;
    color_t g = 0;
    color_t b = 0;
    unsigned int count = 0;
    for(int i = 0; i < 8; i++){
        if(node->children[i] == nullptr) continue;
        r +=  node->children[i]->red();
        g +=  node->children[i]->green();
        b +=  node->children[i]->blue();
        count += node->children[i]->pixelCount();
        leafNums--;
        node->children[i] = nullptr;
    } 

    node->setIsLeaf(true);
    node->setRed(r);
    node->setBlue(b);
    node->setGreen(g);
    node->setPixelCount(count);
    leafNums++;
}

void Octree::colorStats(ColorNode* node, vector<ColorCount*> &colors){
    if(node->Leaf()){
        color_t r = node->red() / node->pixelCount();
        color_t g = node->green() / node->pixelCount();
        color_t b = node->blue() / node->pixelCount();

        ColorCount* cnt = new ColorCount();
        cnt->count = node->pixelCount();
        cnt->color[0] = r;
        cnt->color[1] = g;
        cnt->color[2] = b;
        cnt->colorValue = (r << 16) + (g << 8) + b;
        colors.push_back(cnt);
        return;
    }

    for (size_t i = 0; i < 8; i++)
    {
        if(nullptr != node->children[i] ){
            colorStats(node->children[i],colors);
        }
    }
    
    if(root == node){
        sort(colors.begin(),colors.end(),ColorCount::cmp);
    }
}

void Octree::buildOctree(vector<RGB*> &color, int maxColor){
    for (auto iter = color.begin(); iter != color.end(); iter++)
    {
        addColor(root,(*iter),0);
        while (leafNums > maxColor) reduceTree(); 
    }   
}


//  #define EXTERN_C extern "C"

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
// void free_buf(void* buf) {
// 	free(buf);
// }