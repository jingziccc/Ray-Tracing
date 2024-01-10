#pragma once
#ifndef INTERVAL_H
#define INTERVAL_H

// 用于表示区间的类
class interval {
public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {}

    interval(double _min, double _max) : min(_min), max(_max) {}

    interval(const interval& a, const interval& b)
        : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

    double size() const {
        return max - min;
    }

    // 返回扩大delta后的区间(上下限各扩大delta/2)
    interval expand(double delta) const {
        auto padding = delta / 2;
        return interval(min - padding, max + padding);
    }

    // 判断x是否在区间内(包括上下限)
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    // 判断x是否在区间内(不包括上下限)
    bool surrounds(double x) const {
        return min < x && x < max;
    }

    // 将x限制在区间内(包括上下限)
    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

interval operator+(const interval& ival, double displacement) {
    return interval(ival.min + displacement, ival.max + displacement);
}

interval operator+(double displacement, const interval& ival) {
    return ival + displacement;
}


#endif