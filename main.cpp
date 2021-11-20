#include "./main.hpp"
#include "./Delaunay.hpp"
#include "./DouglasPeucker.hpp"
#include "./FFT.hpp"

int main() {
    Mat srcImage = imread("/Users/xby/CLionProjects/FourierDraw/test.png");
    if (!srcImage.data) {
        printf("Image not loaded");
        return -1;
    }
    Size size = srcImage.size();
    Rect rect(0, 0, size.width, size.height);
    //颜色空间转换
    //去噪音
    blur(srcImage, srcImage, Size(5, 5));
    // 转成灰度图
    cvtColor(srcImage, srcImage, COLOR_BGR2GRAY);
    // 自适应阈值二值化
    adaptiveThreshold(~srcImage, srcImage, 255, ADAPTIVE_THRESH_MEAN_C,
                      THRESH_BINARY, 11, -2);
    imshow("before", srcImage);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(srcImage, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE,
                 Point());
    vector<Point> points;
    vector<D_Point> douglas;
    for (int i = 0; i < contours.size(); i++)
        for (int j = 0; j < contours[i].size(); j++)
            points.push_back(Point(contours[i][j].x, contours[i][j].y));
    DouglasPeucker(points, douglas, 0, points.size() - 1, 0.01);
    points.clear();
    Delaunay dela;
    dela.init(douglas);
    std::vector<std::pair<int, int>> test = dela.getEdge();
    for (int i = 0; i < test.size(); i++)
        line(srcImage,
             Point(douglas[test[i].first].x, douglas[test[i].first].y),
             Point(douglas[test[i].second].x, douglas[test[i].second].y),
             Scalar(255), 1, CV_AA, 0);
    imshow("test1", srcImage);
    waitKey(0);

    return 0;
}