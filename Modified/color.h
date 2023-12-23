#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include <iostream>

using color = vec3;  // ʵ����������ά������ʾ��ɫ

// ���ڽ�������ɫת��Ϊgamma��ɫ
inline double linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

// ����ɫת����0-255��������Χ�ڣ���д���ַ���
void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) 
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // ���Բ���������ȡƽ��ֵ
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // ��������ɫת��Ϊgamma��ɫ
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // ����ɫת����0-255��������Χ�ڣ���д���ַ���
    static const interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}


#endif
