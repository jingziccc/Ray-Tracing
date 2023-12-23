#ifndef INTERVAL_H
#define INTERVAL_H

#ifndef infinity
#define infinity HUGE_VAL
#endif // infinity

// 用于表示区间的类
class interval {
public:
    double min, max; //区间的上下限

    interval() : min(+infinity), max(-infinity){};  // 默认的区间为空
    interval(double _min, double _max); //构造函数

    double size() const; //返回区间的大小
    interval expand(double delta) const; //返回扩大delta后的区间(上下限各扩大delta/2)
    bool contains(double x) const; //判断x是否在区间内(包括上下限)
    bool surrounds(double x) const; //判断x是否在区间内(不包括上下限)
    double clamp(double x) const; //将x限制在区间内(包括上下限)

    static const interval empty, universe;
};

// 静态成员变量的初始化
const interval interval::empty    = interval(+infinity, -infinity); //空区间(无穷大到无穷小)
const interval interval::universe = interval(-infinity, +infinity); //全区间(无穷小到无穷大)

interval::interval(double min, double max)
{
    this->min = min;
    this->max = max;
}

// 返回区间的大小
double interval::size() const
{
    return max - min;
}

// 返回扩大delta后的区间(上下限各扩大delta/2)
interval interval::expand(double delta) const
{
    auto padding = delta/2;
    return interval(min - padding, max + padding);
}

// 判断x是否在区间内(包括上下限)
bool interval::contains(double x) const
{
    return min <= x && x <= max;
}

// 判断x是否在区间内(不包括上下限)
bool interval::surrounds(double x) const
{
    return min < x && x < max;
}

// 将x限制在区间内(包括上下限)
double interval::clamp(double x) const
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#endif
