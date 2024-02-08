#pragma once
#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include "Color.hpp"
#include <vector>
#include <math.h>

template <typename T>
class vect{
    public:
        T one;
        T two;
        T three;

        vect():one(0),two(0),three(0){};

        vect(T o, T tw, T th):one(o),two(tw),three(th){};
};

class Kmeans{
    public:
        vector<vect<double> *> vect_space;
        vector<vect<color_t> *> cluster;
        int vect_count;

        //生成n个随机数
        static size_t generateRandomA(int n, size_t RangeL, size_t RangeR)
        {

            if (RangeL > RangeR)
            {
                int temp = RangeL;
                RangeL = RangeR;
                RangeR = temp;
            }

            srand((unsigned)time(NULL));

            return (size_t)(rand() % (RangeR - RangeL + 1) + RangeL);
        }

        Kmeans(int count = 8){
            vect_count = count;
            vect_space = new vector<vect<double> *>(vect_count, nullptr);
            cluster = new vector<vect<color_t> *>(vect_count, nullptr);
        }

        //计算颜色向量和空间向量的距离
        // color_vect:颜色向量，space_vect:向量空间，type_d: 距离类型 -> true: 欧式距离；false: 曼哈顿距离
        vect<double>*  calcu_vc_d(RGB* color_vect, vect<double>* space_vect, bool type_d ){
            vect<double>* temp = new vect<double>();

            double ou_calcu(double f,double s){
                return sqrt(fabs(pow((f - s),2)));
            }

            double mh_calcu(double f,double s){
                return fabs(f - s);
            }

            if(type_d){
                temp->one = ou_calcu((double)color_vect->r, space_vect->one);
                temp->one = ou_calcu((double)color_vect->g, space_vect->two);
                temp->one = ou_calcu((double)color_vect->b, space_vect->three);
            }else{
                temp->one = mh_calcu((double)color_vect->r, space_vect->one);
                temp->one = mh_calcu((double)color_vect->g, space_vect->two);
                temp->one = mh_calcu((double)color_vect->b, space_vect->three);
            }

            return temp;
        }



};