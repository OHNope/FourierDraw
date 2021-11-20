//
// Created by XBY on 2021/11/19.
//

#ifndef FOURIERDRAW_DOUGLASPEUCKER_HPP
#define FOURIERDRAW_DOUGLASPEUCKER_HPP

float fast_inverse_sqrt(float x) {
    float half_x = 0.5 * x;
    int i = *((int *)&x);             // 以整数方式读取X
    i = 0x5f3759df - (i >> 1);        // 神奇的步骤
    x = *((float *)&i);               // 再以浮点方式读取i
    x = x * (1.5 - (half_x * x * x)); // 牛顿迭代一遍提高精度
    return x;
}

double perpendicularDistance(Point curPoint, double A, double B, double C) {
    double distance = (A * curPoint.x + B * curPoint.y + C) *
                      fast_inverse_sqrt(A * A + B * B);
    return distance < 0 ? -distance : distance;
}

// DouglasPeucker 算法
void DouglasPeucker(vector<Point> &Points, vector<D_Point> &newPoints,
                    int Start, int End, double epsilon) {
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
        DouglasPeucker(Points, newPoints, Start, index, epsilon);
        DouglasPeucker(Points, newPoints, index, End, epsilon);
    } else {
        newPoints.push_back(
            D_Point(Points[Start].x, Points[Start].y, newPoints.size()));
        newPoints.push_back(
            D_Point(Points[End].x, Points[End].y, newPoints.size()));
    }
}

#endif // FOURIERDRAW_DOUGLASPEUCKER_HPP
