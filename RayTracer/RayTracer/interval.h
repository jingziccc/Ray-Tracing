#pragma once
#ifndef INTERVAL_H
#define INTERVAL_H

// ���ڱ�ʾ�������
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

    // ��������delta�������(�����޸�����delta/2)
    interval expand(double delta) const {
        auto padding = delta / 2;
        return interval(min - padding, max + padding);
    }

    // �ж�x�Ƿ���������(����������)
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    // �ж�x�Ƿ���������(������������)
    bool surrounds(double x) const {
        return min < x && x < max;
    }

    // ��x������������(����������)
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