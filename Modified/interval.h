#ifndef INTERVAL_H
#define INTERVAL_H

#ifndef infinity
#define infinity HUGE_VAL
#endif // infinity

// ���ڱ�ʾ�������
class interval {
public:
    double min, max; //�����������

    interval() : min(+infinity), max(-infinity){};  // Ĭ�ϵ�����Ϊ��
    interval(double _min, double _max); //���캯��

    double size() const; //��������Ĵ�С
    interval expand(double delta) const; //��������delta�������(�����޸�����delta/2)
    bool contains(double x) const; //�ж�x�Ƿ���������(����������)
    bool surrounds(double x) const; //�ж�x�Ƿ���������(������������)
    double clamp(double x) const; //��x������������(����������)

    static const interval empty, universe;
};

// ��̬��Ա�����ĳ�ʼ��
const interval interval::empty    = interval(+infinity, -infinity); //������(���������С)
const interval interval::universe = interval(-infinity, +infinity); //ȫ����(����С�������)

interval::interval(double min, double max)
{
    this->min = min;
    this->max = max;
}

// ��������Ĵ�С
double interval::size() const
{
    return max - min;
}

// ��������delta�������(�����޸�����delta/2)
interval interval::expand(double delta) const
{
    auto padding = delta/2;
    return interval(min - padding, max + padding);
}

// �ж�x�Ƿ���������(����������)
bool interval::contains(double x) const
{
    return min <= x && x <= max;
}

// �ж�x�Ƿ���������(������������)
bool interval::surrounds(double x) const
{
    return min < x && x < max;
}

// ��x������������(����������)
double interval::clamp(double x) const
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#endif
