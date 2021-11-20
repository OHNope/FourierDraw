//
// Created by XBY on 2021/11/19.
//

#ifndef FOURIERDRAW_DOUGLASPEUCKER_HPP
#define FOURIERDRAW_DOUGLASPEUCKER_HPP

typedef struct stPoint {
    double x;
    double y;
    int isSaved; // 是否为曲线拟合的保留点

    stPoint(double a = 0, double b = 0, int c = 0) : x(a), y(b), isSaved(c){};
} stPoint;

float fast_inverse_sqrt(float x) {
    float half_x = 0.5 * x;
    int i = *((int *)&x);             // 以整数方式读取X
    i = 0x5f3759df - (i >> 1);        // 神奇的步骤
    x = *((float *)&i);               // 再以浮点方式读取i
    x = x * (1.5 - (half_x * x * x)); // 牛顿迭代一遍提高精度
    return x;
}

double perpendicularDistance(stPoint curPoint, double A, double B, double C) {
    double distance = (A * curPoint.x + B * curPoint.y + C) *
                      fast_inverse_sqrt(A * A + B * B);
    return distance < 0 ? -distance : distance;
}

// DouglasPeucker 算法
int DouglasPeucker(vector<stPoint> &Points, int Start, int End,
                   double epsilon) {
    double dMax = 0;
    int index = 0;
    int iter;

    // 直线方程： Ax + By + C = 0
    double A, B, C;
    if (Points[Start].x == Points[End].x) { // 垂直x轴的直线
        A = 1;
        B = 0;
        C = -Points[Start].x;
    } else {                                    // 常规直线
        A = Points[End].y - Points[Start].y;    // y1 - y0
        B = -(Points[End].x - Points[Start].x); // -(x1 - x0)
        C = Points[Start].y * (-B) - Points[Start].x * A;
    }
    for (iter = Start + 1; iter < End; ++iter) {
        // 点到直线的垂直距离
        double distance = perpendicularDistance(Points[iter], A, B, C);
        if (distance > dMax) {
            dMax = distance;
            index = iter;
        }
    }

    if (dMax > epsilon) {
        // cout << "dMax Distance Index = " << index << endl;
        DouglasPeucker(Points, Start, index, epsilon);
        DouglasPeucker(Points, index, End, epsilon);
    } else {
        (Points[Start]).isSaved = 1;
        (Points[End]).isSaved = 1;
    }
    return 0;
}

#endif //FOURIERDRAW_DOUGLASPEUCKER_HPP
