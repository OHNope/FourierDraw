//
// Created by XBY on 2021/11/20.
//

#ifndef FOURIERDRAW_KRUSKAL_HPP
#define FOURIERDRAW_KRUSKAL_HPP

/*int par[MAXN], Rank[MAXN];
typedef struct {
    int a, b, price;
} Node;
Node a[MAXN];*/

int cmp(const Point *a, const Point *b) {
    return ((Point *)a)->price - ((Node *)b)->price;
}
void Init(int n) {
    for (int i = 0; i < n; i++) {
        Rank[i] = 0;
        par[i] = i;
    }
}

int find(int x) {
    int root = x;
    while (root != par[root])
        root = par[root];
    while (x != root) {
        int t = par[x];
        par[x] = root;
        x = t;
    }
    return root;
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
int Kruskal(int n) {
    int res = 0;
    //将边按照权值从小到大排序
    qsort(a, n, sizeof(a[0]), cmp);
    while (n > 1) {
        //判断当前这条边的两个端点是否属于同一棵树
        if (find(a[i].a) != find(a[i].b)) {
            unite(a[i].a, a[i].b);
            res += a[i].price;
            nEdge++;
        }
    }
    //如果加入边的数量小于m - 1,则表明该无向图不连通,等价于不存在最小生成树
    if (nEdge < m - 1)
        res = -1;
    return res;
}

#endif // FOURIERDRAW_KRUSKAL_HPP
