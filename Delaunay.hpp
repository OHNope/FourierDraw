//
// Created by XBY on 2021/11/19.
//

#ifndef FOURIERDRAW_DELAUNAY_HPP
#define FOURIERDRAW_DELAUNAY_HPP

// Draw delaunay triangles

static void delaunay(Mat &img, Subdiv2D &subdiv, Scalar delaunay_color) {
    vector<Vec6f> triangleList;

    subdiv.getTriangleList(triangleList);

    vector<Point> pt(3);
    Size size = img.size();
    Rect rect(0, 0, size.width, size.height);

    for (size_t i = 0; i < triangleList.size(); i++) {
        Vec6f t = triangleList[i];
        pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));

        // Draw rectangles completely inside the image.
        if (rect.contains(pt[0]) && rect.contains(pt[1]) &&
            rect.contains(pt[2])) {
            line(img, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
            line(img, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
            line(img, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);
        }
    }
}

#endif // FOURIERDRAW_DELAUNAY_HPP
