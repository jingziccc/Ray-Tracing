#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include <iostream>

using color = vec3;  // 实际上是用三维向量表示颜色

// 用于将线性颜色转换为gamma颜色
inline double linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

// 将颜色转换到0-255的整数范围内，并写入字符流
void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) 
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // 除以采样次数，取平均值
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // 从线性颜色转换为gamma颜色
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // 将颜色转换到0-255的整数范围内，并写入字符流
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}


#endif
