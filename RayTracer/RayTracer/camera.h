#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"
#include "color.h"
#include "hittable.h"
#include "material.h"

#include <vector>
#include <thread>
#include <mutex>

#include <iostream>

class camera
{
public:
    // 目标宽长比
    double aspectRatio = 1.0;
    // 渲染图片默认宽度
    int imageWidth = 100;
    // 渲染采样个数
    int spp = 10;
    // 光线最大反射次数
    int maxDepth = 10;

    // 垂直视角
    double vfov = 90;
    point3 lookfrom = point3(0, 0, -1);
    point3 lookat = point3(0, 0, 0);
    vec3 vup = vec3(0, 1, 0);

    // 焦距
    double focuesDistance = 1;

    // 渲染函数
    void render(const hittable& world)
    {
        // 初始化
        initialize();

        // 多线程渲染
        std::vector<color> framebuffer(imageWidth * imageHeight);
        const int threadsNum = 32;
        std::thread th[threadsNum];
        int threadRows = imageHeight / threadsNum;

        auto run = [&](uint32_t startRow, uint32_t endRow)
            {
                for (uint32_t j = startRow; j < endRow; j++)
                {
                    for (uint32_t i = 0; i < imageWidth; i++)
                    {
                        color pixelColor(0, 0, 0);
                        for (int sample = 0; sample < spp; sample++)
                        {
                            ray r = sampleRay(i, j);
                            pixelColor += castRay(r, maxDepth, world);
                            framebuffer[j * imageWidth + i] = pixelColor;
                        }
                    }
                    mtx.lock();
                    progress++;
                    std::clog << "\rScanlines remaining: " << (imageHeight - progress) << ' ' << std::flush;
                    mtx.unlock();
                }
            };

        for (int i = 0; i < threadsNum; i++)
        {
            th[i] = std::thread(run, i * threadRows, (i + 1) * threadRows);
        }

        // 如果存在未被线程覆盖的行
        int lastRow = threadRows * threadsNum;
        if (lastRow != imageHeight)
        {
            for (uint32_t j = lastRow - 1; j < imageHeight; ++j)
            {
                for (uint32_t i = 0; i < imageWidth; ++i)
                {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < spp; ++sample)
                    {
                        ray r = sampleRay(i, j);
                        pixel_color += castRay(r, maxDepth, world);
                        framebuffer[j * imageWidth + i] = pixel_color;
                    }
                }
                mtx.lock();
                progress++;
                std::clog << "\rScanlines remaining: " << (imageHeight - progress) << ' ' << std::flush;
                mtx.unlock();
            }
        }
        for (int i = 0; i < threadsNum; i++)
        {
            th[i].join();
        }

        // 将framebuffer中颜色写入ppm文件
        std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
        for (int j = 0; j < imageHeight; ++j) {
            // std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < imageWidth; ++i) {
                write_color(std::cout, framebuffer[j * imageWidth + i], spp);
            }
        }
        std::clog << "\rDone.                  \n";
    }

private:
        std::mutex mtx;
        int progress = 0;

        int imageHeight;
        point3 camPos;
        point3 leftTopPixelPos;
        vec3 deltaU;
        vec3 deltaV;

        vec3 u, v, w;

        // asas
        void initialize()
        {
            imageHeight = (int)(imageWidth / aspectRatio);
            // 保证图片长度最少为一个像素
            imageHeight = (imageHeight < 1) ? 1 : imageHeight;

            camPos = lookfrom;
            auto theta = degrees_to_radians(vfov);

            // 根据vfov计算视口长宽
            auto h = tan(theta / 2);
            auto viewportHeight = 2 * h * focuesDistance;
            auto veiwportWidth = viewportHeight * ((double)imageWidth / imageHeight);

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            vec3 viewportU = veiwportWidth * u;
            // 图像像素纵轴向下增长
            vec3 viewportV = viewportHeight * -v;

            deltaU = viewportU / imageWidth;
            deltaV = viewportV / imageHeight;

            auto leftTopPortPos = camPos - w * focuesDistance - viewportU / 2 - viewportV / 2;
            leftTopPixelPos = leftTopPortPos + (deltaU + deltaV) / 2;
        }

        // 抗锯齿，在每个像素点周围[-0.5, 0.5]的正方形区域内进行采样
        ray sampleRay(int i, int j) const
        {
            auto pixelCurrentPos = leftTopPixelPos + (i * deltaU) + (j * deltaV);
            auto pixelSamplePos = pixelCurrentPos + pixelSampleSquare();

            auto rayOrigin = camPos;
            auto rayDirection = pixelSamplePos - rayOrigin;

            return ray(rayOrigin, rayDirection);
        }

        // 方形采样
        vec3 pixelSampleSquare() const
        {
            // 随机返回正方形上的点
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();
            return (px * deltaU) + (py * deltaV);
        }

        // 射出光线
        vec3 castRay(const ray & r, int depth, const hittable & world) const
        {
            if (depth <= 0)
                return color(1, 1, 1);

            hit_payload payload;

            if (world.hit(r, interval(0.001, infinity), payload)) {
                ray newRay;
                color pixelColor;

                if (payload.mat->castray(r, payload, pixelColor, newRay))
                    return pixelColor * castRay(newRay, depth - 1, world);

                // 停止反射，返回0
                return color(1, 1, 1);
            }

            vec3 unitDirection = unit_vector(r.direction());
            auto a = 0.5 * (unitDirection.y() + 1.0);
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }
};
#endif