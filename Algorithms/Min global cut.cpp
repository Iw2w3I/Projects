#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <tuple>

//https://codeforces.com/group/PVbQ8eK2T4/contest/379712/problem/G
//complexity EV^3

template <typename T>
class Graph {
private:
    std::vector<std::vector<T>> g;
public:
    Graph(long long n) {
        g = std::vector<std::vector<T>> (n);
    }

    Graph(long long n, long long m) {
        g = std::vector<std::vector<T>> (n, std::vector<T> (m));
    }

    std::vector<T>& operator[] (long long i) {
        return g[i];
    }

    long long size() const {
        return g.size();
    }
};

void bfs(long long v, Graph<std::tuple<long long, long long, long long>>& g, std::vector<long long>& dist) {
    dist[v] = 0;
    std::queue<long long> q;
    q.push(v);
    while (!q.empty()) {
        long long w = q.front();
        q.pop();
        for (long long i = 0; i < g[w].size(); ++i)
            if (get<0>(g[w][i]) && dist[i] == std::numeric_limits<long long>::max() &&
                get<1>(g[w][i]) != get<2>(g[w][i])) {
                dist[i] = dist[w] + 1;
                q.push(i);
            }
    }
}

long long dfs(long long start, long long finish, long long f, Graph<std::tuple<long long, long long, long long>>& g,
              std::vector<long long>& ptr, std::vector<long long>& dist) {
    if (start == finish)
        return f;
    while (ptr[start] < g[start].size()) {
        auto v = g[start][ptr[start]];
        if (get<0>(v) && dist[start] + 1 == dist[ptr[start]] && get<1>(v) != get<2>(v)) {
            long long delta = dfs(ptr[start], finish, std::min(f, get<1>(v) - get<2>(v)),g, ptr, dist);
            if (delta != 0) {
                get<2>(g[start][ptr[start]]) += delta;
                get<2>(g[ptr[start]][start]) -= delta;
                return delta;
            }
        }
        ++ptr[start];
    }
    return 0;
}

long long max_flow(long long start, long long finish, Graph<std::tuple<long long, long long, long long>> g,
                   std::vector<long long>& dist, long long inf) {
    long long ans = 0;
    bfs(start, g, dist);
    while (dist[finish] != inf) {
        std::vector<long long> ptr(g.size(), 0);
        long long flow = dfs(start, finish, inf, g, ptr, dist);
        while (flow != 0) {
            ans += flow;
            flow = dfs(start, finish, inf, g, ptr, dist);
        }
        dist = std::vector<long long> (g.size(), inf);
        bfs(start, g, dist);
    }
    return ans;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    long long n, inf = std::numeric_limits<long long>::max();
    char a;
    std::cin >> n;
    Graph<std::tuple<long long, long long, long long>> g(n, n);
    for (long long i = 0; i < n; ++i)
        for (long long j = 0; j < n; ++j) {
            std::cin >> a;
            if (a == '1')
                g[i][j] = {1, 1, 0};
            else
                g[i][j] = {0, 0, 0};
        }
    long long min_ans = inf, min_i;
    std::vector<long long> flows(n);
    for (long long i = 1; i < n; ++i) {
        std::vector<long long> dist(n, inf);
        flows[i] = max_flow(0, i, g, dist, inf);
        if (flows[i] < min_ans) {
            min_ans = flows[i];
            min_i = i;
        }
    }
    std::vector<long long> dist(n, inf);
    max_flow(0, min_i, g, dist, inf);
    for (long long i = 0; i < n; ++i)
        if (dist[i] != inf)
            std::cout << i + 1 << " ";
    std::cout << std::endl;
    for (long long i = 0; i < n; ++i)
        if (dist[i] == inf)
            std::cout << i + 1 << " ";
    return 0;
}
