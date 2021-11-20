//
// Created by XBY on 2021/11/19.
//

#ifndef FOURIERDRAW_DELAUNAY_HPP
#define FOURIERDRAW_DELAUNAY_HPP
#include <algorithm>
#include <cmath>
#include <cstring>
#include <list>
#include <utility>
#include <vector>

const double EPS = 1e-8;
const int MAXV = 10000;

struct D_Point {
    double x, y;
    int id;

    D_Point(double a = 0, double b = 0, int c = -1) : x(a), y(b), id(c) {}

    bool operator<(const D_Point &a) const {
        return x < a.x || (fabs(x - a.x) < EPS && y < a.y);
    }

    bool operator==(const D_Point &a) const {
        return fabs(x - a.x) < EPS && fabs(y - a.y) < EPS;
    }

    double dist2(const D_Point &b) {
        return (x - b.x) * (x - b.x) + (y - b.y) * (y - b.y);
    }
};

struct D_Point3D {
    double x, y, z;

    D_Point3D(double a = 0, double b = 0, double c = 0) : x(a), y(b), z(c) {}

    D_Point3D(const D_Point &p) { x = p.x, y = p.y, z = p.x * p.x + p.y * p.y; }

    D_Point3D operator-(const D_Point3D &a) const {
        return D_Point3D(x - a.x, y - a.y, z - a.z);
    }

    double dot(const D_Point3D &a) { return x * a.x + y * a.y + z * a.z; }
};

struct Edge {
    int id;
    std::list<Edge>::iterator c;
    Edge(int id = 0) { this->id = id; }
};

int cmp(double v) { return fabs(v) > EPS ? (v > 0 ? 1 : -1) : 0; }

double cross(const D_Point &o, const D_Point &a, const D_Point &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

D_Point3D cross(const D_Point3D &a, const D_Point3D &b) {
    return D_Point3D(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x,
                     a.x * b.y - a.y * b.x);
}

int inCircle(const D_Point &a, D_Point b, D_Point c, const D_Point &p) {
    if (cross(a, b, c) < 0)
        std::swap(b, c);
    D_Point3D a3(a), b3(b), c3(c), p3(p);
    b3 = b3 - a3, c3 = c3 - a3, p3 = p3 - a3;
    D_Point3D f = cross(b3, c3);
    return cmp(p3.dot(f)); // check same direction, in: < 0, on: = 0, out: > 0
}

int intersection(const D_Point &a, const D_Point &b, const D_Point &c,
                 const D_Point &d) { // seg(a, b) and seg(c, d)
    return cmp(cross(a, c, b)) * cmp(cross(a, b, d)) > 0 &&
           cmp(cross(c, a, d)) * cmp(cross(c, d, b)) > 0;
}

class Delaunay {
public:
    std::list<Edge> head[MAXV]; // graph
    D_Point p[MAXV];
    int n, rename[MAXV];

    void init(const vector<D_Point> &in) {
        memcpy(this->p, p, sizeof(D_Point) * n);
        for (int i = 0; i < in.size(); i++)
            this->p[i] = in[i];
        std::sort(this->p, this->p + n);
        this->n = in.size();
        divide(0, n - 1);
    }

    void addEdge(int u, int v) {
        head[u].push_front(Edge(v));
        head[v].push_front(Edge(u));
        head[u].begin()->c = head[v].begin();
        head[v].begin()->c = head[u].begin();
    }

    void divide(int l, int r) {
        if (r - l <= 2) { // #D_Point <= 3
            for (int i = l; i <= r; i++)
                for (int j = i + 1; j <= r; j++)
                    addEdge(i, j);
            return;
        }
        int mid = (l + r) / 2;
        divide(l, mid);
        divide(mid + 1, r);

        std::list<Edge>::iterator it;
        int nowl = l, nowr = r;

        for (int update = 1; update;) {
            // find left and right convex, lower common tangent
            update = 0;
            D_Point ptL = p[nowl], ptR = p[nowr];
            for (it = head[nowl].begin(); it != head[nowl].end(); it++) {
                D_Point t = p[it->id];
                double v = cross(ptR, ptL, t);
                if (cmp(v) > 0 ||
                    (cmp(v) == 0 && ptR.dist2(t) < ptR.dist2(ptL))) {
                    nowl = it->id, update = 1;
                    break;
                }
            }
            if (update)
                continue;
            for (it = head[nowr].begin(); it != head[nowr].end(); it++) {
                D_Point t = p[it->id];
                double v = cross(ptL, ptR, t);
                if (cmp(v) < 0 ||
                    (cmp(v) == 0 && ptL.dist2(t) < ptL.dist2(ptR))) {
                    nowr = it->id, update = 1;
                    break;
                }
            }
        }

        addEdge(nowl, nowr); // add tangent

        for (int update = 1; true;) {
            update = 0;
            D_Point ptL = p[nowl], ptR = p[nowr];
            int ch = -1, side = 0;
            for (it = head[nowl].begin(); it != head[nowl].end(); it++) {
                if (cmp(cross(ptL, ptR, p[it->id])) > 0 &&
                    (ch == -1 || inCircle(ptL, ptR, p[ch], p[it->id]) < 0)) {
                    ch = it->id, side = -1;
                }
            }
            for (it = head[nowr].begin(); it != head[nowr].end(); it++) {
                if (cmp(cross(ptR, p[it->id], ptL)) > 0 &&
                    (ch == -1 || inCircle(ptL, ptR, p[ch], p[it->id]) < 0)) {
                    ch = it->id, side = 1;
                }
            }
            if (ch == -1)
                break; // upper common tangent
            if (side == -1) {
                for (it = head[nowl].begin(); it != head[nowl].end();) {
                    if (intersection(ptL, p[it->id], ptR, p[ch])) {
                        head[it->id].erase(it->c);
                        head[nowl].erase(it++);
                    } else {
                        it++;
                    }
                }
                nowl = ch;
                addEdge(nowl, nowr);
            } else {
                for (it = head[nowr].begin(); it != head[nowr].end();) {
                    if (intersection(ptR, p[it->id], ptL, p[ch])) {
                        head[it->id].erase(it->c);
                        head[nowr].erase(it++);
                    } else {
                        it++;
                    }
                }
                nowr = ch;
                addEdge(nowl, nowr);
            }
        }
    }

    std::vector<std::pair<int, int>> getEdge() {
        std::vector<std::pair<int, int>> ret;
        ret.reserve(n);
        std::list<Edge>::iterator it;
        for (int i = 0; i < n; i++) {
            for (it = head[i].begin(); it != head[i].end(); it++) {
                if (it->id < i)
                    continue;
                ret.push_back(std::make_pair(p[i].id, p[it->id].id));
            }
        }
        return ret;
    }
};

// Draw delaunay triangles

#endif // FOURIERDRAW_DELAUNAY_HPP
