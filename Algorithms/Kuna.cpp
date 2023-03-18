#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <tuple>

//https://codeforces.com/group/PVbQ8eK2T4/contest/379712/problem/F
//complexity ANS*(N+M)

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

bool augment(long long v, std::vector<bool>& used, std::vector<long long>& match, Graph<long long>& g) {
    if (used[v])
        return false;
    used[v] = true;
    for (long long t: g[v]) {
        if (match[t] == -1 || augment(match[t], used, match, g)) {
            match[t] = v;
            return true;
        }
    }
    return false;
}

void dfs(long long v, std::vector<long long>& tin, std::vector<long long>& tout,
         Graph<long long>& g, std::vector<bool>& used, long long& timer) {
    tin[v] = timer++;
    used[v] = true;
    for (long long t: g[v]) {
        if (used[t])
            continue;
        dfs(t, tin, tout, g, used, timer);
    }
    tout[v] = timer++;
}

void dfs(long long v, std::vector<bool>& used, Graph<long long>& g) {
    std::vector<long long> tin(g.size());
    std::vector<long long> tout(g.size());
    long long timer = 0;
    dfs(v, tin, tout, g, used, timer);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    long long m, n, a, b;
    std::cin >> n >> m;
    Graph<long long> g(n * 2);
    Graph<long long> j(n);
    for (long long i = 0; i < m; ++i) {
        std::cin >> a >> b;
        g[a - 1].push_back(b - 1 + n);
        j[a - 1].push_back(b - 1);
    }
    std::vector<bool> used(n, false);
    std::vector<long long> match(2 * n, -1);
    for (long long i = 0; i < n; ++i)
        if (augment(i, used, match, g))
            used = std::vector<bool>(n, false);
    long long ans = 0;
    for (long long i = 0; i < n; ++i)
        if (match[i + n] == -1)
            ++ans;
    std::cout << ans;
    return 0;
}
