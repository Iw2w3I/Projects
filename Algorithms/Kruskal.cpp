#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <tuple>

//https://codeforces.com/group/PVbQ8eK2T4/contest/379712/problem/B
//complexity M*logM

template <typename T>
class Graph {
private:
    std::vector<std::vector<T>> g;
public:
    Graph(long long n) {
        g = std::vector<std::vector<T>> (n);
    }

    std::vector<T>& operator[] (long long i) {
        return g[i];
    }

    long long size() const {
        return g.size();
    }
};

class Dsu {
private:
    std::vector<long long> g;
public:
    Dsu(long long n) {
        g = std::vector<long long> (n);
        for (long long i = 0; i < n; ++i)
            g[i] = -1;
    }

    bool in_one_set(long long v, long long u) {
        return get(v) == get(u);
    }

    void unite(long long v, long long u) {
        long long get_v = get(v), get_u = get(u);
        if (get_v != get_u) {
            if (size(get_v) < size(get_u)) {
                std::swap(u,v);
                std::swap(get_u,get_v);
            }
            long long copy;
            while (v >= 0) {
                copy = g[v];
                g[v] = get_u;
                v = copy;
            }
        }
    }

    long long get(long long v) {
        if (g[v] < 0)
            return v;
        else
            return get(g[v]);
    }

    long long size(long long v) {
        return -g[get(v)];
    }
};


int main() {
    std::ios::sync_with_stdio(false); //min cost tree
    std::cin.tie(0);
    std::cout.tie(0);
    long long m, n;
    std::cin >> n >> m;
    Graph<std::pair<long long, long long>> g(n);
    std::priority_queue<std::tuple<long long, long long, long long>,
            std::vector<std::tuple<long long, long long, long long>>, std::greater<>> q;
    Dsu d(n);
    long long a, b, w;
    for (long long i = 0; i < m; ++i) {
        std::cin >> a >> b >> w;
        q.push({w, a - 1, b - 1});
    }
    long long ans = 0;
    while (!q.empty()) {
        auto v = q.top();
        q.pop();
        if (!d.in_one_set(get<1>(v), get<2>(v))) {
            ans += get<0>(v);
            d.unite(get<1>(v), get<2>(v));
        }
    }
    std::cout << ans;
    return 0;
}
