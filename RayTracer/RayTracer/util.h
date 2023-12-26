#pragma once
#ifndef RTWEEKEND_H
#define RTWEEKEND_H
//==============================================================================================
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>


using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// 常数

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// 工具函数

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // 随机返回 [0,1) 上的实数.
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // 随机返回[min,max)上的实数.
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
    // 随机返回 [min,max]上的整型数
    return static_cast<int>(random_double(min, max + 1));
}

#include "interval.h"
#include "ray.h"
#include "vec3.h"


#endif