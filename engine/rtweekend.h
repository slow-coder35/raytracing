#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include<iostream>
#include<vector>
#include<memory>
#include<limits>
#include<random>




using std::shared_ptr;
using std::make_shared;


const double infinity=std::numeric_limits<double>::infinity();
const double pi=3.1415926535897932385;




inline double degree_to_radians(double degrees){
    return (degrees*pi/180.0);
}

inline double random_double(){

    thread_local std::mt19937 generator(std::random_device{}());
    thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);

    return distribution(generator);
}

inline double random_double(double min,double max){
    return min+ (max-min)*random_double();
}
inline int random_int(int min, int max){
    return int(random_double(min,max+1));
}

#include"interval.h"
#include"ray.h"
#include"vec3.h"
#include"color.h"
#include"camera.h"
#include "aabb.h"
#include "texture.h"


#endif
