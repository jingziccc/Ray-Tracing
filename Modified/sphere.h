#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

// ������
class sphere: public hitable  {
public:
    vec3 center; //����
    float radius; //�뾶
    material *mat_ptr; //����

    sphere(){}; //Ĭ�Ϲ��캯��
    sphere(vec3 center, float radius, material* mat_ptr); //���캯��

    //�жϹ����Ƿ��������ཻ
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;    
};

sphere::sphere(vec3 center, float radius, material *mat_ptr)
{
    this->center = center;
    this->radius = radius;
    this->mat_ptr = mat_ptr;
}

/**
 * @brief �жϹ����Ƿ��������ཻ����O+tD�ķ�ʽ��ʾ���ߣ��ж�||O+tD-C||^2=r^2 ||D||^2t^2+2(O-C)Dt+||O-C||^2-r^2=0�Ƿ��н�
 * @param r ����
 * @param t_min t�ķ�Χ����
 * @param t_max t�ķ�Χ����
 * @param rec ��¼�ཻ��Ϣ�Ľṹ��
 * @return true: �ཻ; false: ���ཻ
 */
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const 
{
    // ������㵽���ĵ�����(O-C)
    vec3 oc = r.origin() - center;

    // ���߷���������ģ��ƽ��(D^2)
    float a = dot(r.direction(), r.direction());

    // ������㵽���ĵ���������߷��������ĵ��((O-C)D)
    float b = dot(oc, r.direction());

    // ������㵽���ĵ�������ģ��ƽ����ȥ��뾶��ƽ��(||O-C||^2-r^2)
    float c = dot(oc, oc) - radius*radius;

    // �б�ʽ
    float discriminant = b*b - a*c;

    // �ж��Ƿ��н�
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant))/a; //��С�Ľ�

        // �����С�Ľ��ڴ�����Χ��
        if (temp < t_max && temp > t_min) {
            rec.t = temp; //��¼�ཻ���tֵ
            rec.p = r.point_at_parameter(rec.t); //��¼�ཻ�������
            rec.normal = (rec.p - center) / radius; //��¼�ཻ��ķ�����
            rec.mat_ptr = mat_ptr; //��¼�ཻ��Ĳ���
            return true;
        }

        temp = (-b + sqrt(discriminant)) / a; //�ϴ�Ľ�

        // ����ϴ�Ľ��ڴ�����Χ��
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false; //���ز��ཻ
}

#endif
