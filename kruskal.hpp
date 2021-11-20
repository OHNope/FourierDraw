//
// Created by XBY on 2021/11/20.
//

#ifndef FOURIERDRAW_KRUSKAL_HPP
#define FOURIERDRAW_KRUSKAL_HPP

int find(const int x, const int *father) {
    while (x != father[x])
        x = father[x];
    return x;
}

void unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (Rank[x] < Rank[y]) {
        par[x] = y;
    } else {
        par[y] = x;
        if (Rank[x] == Rank[y])
            Rank[x]++;
    }
}
// n为边的数量
int Kruskal(vector<Point> points, const vector<Edge> &edges) {
    //将边按照权值从小到大排序
    int n = edges.size(), k = 0;
    int *father = new int[points.size()];
    for (int i = 0; i < points.size(); i++)
        father[i] = i;
    qsort(edges, n, sizeof(Edge), [](const Edge *a, const Edge *b) -> bool {
        return a->weights - b->weights;
    });
    for (int i = 0; i < edges.size(); i++) {
        //判断当前这条边的两个端点是否属于同一棵树
        if (find(i * 2, father) != find(i * 2 + 1, father)) {
            unite(edges[i].vtx1, edges[i].vtx2);
        }
    }
    //如果加入边的数量小于m - 1,则表明该无向图不连通,等价于不存在最小生成树
}

#endif // FOURIERDRAW_KRUSKAL_HPP
