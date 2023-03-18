#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <tuple>

//https://codeforces.com/group/PVbQ8eK2T4/contest/379712/problem/E

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

void dfs(long long v, long long p, std::vector<long long>& tin, std::vector<long long>& tout,
         Graph<long long>& g, std::vector<bool>& used, long long& timer) {
    tin[v] = timer++;
    used[v] = true;
    for (long long t: g[v]) {
        if (used[t])
            continue;
        dfs(t, v, tin, tout, g, used, timer);
    }
    tout[v] = timer++;
}

void dfs(long long v, long long p, std::vector<bool>& used, Graph<long long>& g) {
    std::vector<long long> tin(g.size());
    std::vector<long long> tout(g.size());
    long long timer = 0;
    dfs(v, p, tin, tout, g, used, timer);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    long long m, n, a, b;
    std::cin >> m >> n;
    Graph<long long> g(n + m);
    for (long long i = 0; i < m; ++i) {
        std::cin >> a;
        for (long long j = 0; j < a; ++j) {
            std::cin >> b;
            g[i].push_back(b - 1 + m);
        }
    }
    for (long long i = 0; i < m; ++i) {
        std::cin >> a;
        if (a != 0) {
            g[a - 1 + m].push_back(i);
            g[i].erase(find(g[i].begin(), g[i].end(), a - 1 + m));
        }
    }
    std::vector<bool> saturated(m, false);
    for (long long i = m; i < n + m; ++i) {
        for (long long j: g[i])
            saturated[j] = true;
    }
    std::vector<bool> used(m + n, false);
    for (long long i = 0; i < m; ++i) {
        if (!saturated[i]) {
            dfs(i, -1, used, g);
        }
    }
    long long count = 0;
    std::vector<long long> l_ans;
    for (long long i = 0; i < m; ++i) {
        if (!used[i]) {
            l_ans.push_back(i);
            ++count;
        }
    }
    std::vector<long long> r_ans;
    for (long long i = m; i < m + n; ++i) {
        if (used[i]) {
            r_ans.push_back(i);
            ++count;
        }
    }
    std::cout << count << '\n' << l_ans.size() << " ";
    for (long long i: l_ans)
        std::cout << i + 1 << " ";
    std::cout << '\n' << r_ans.size() << " ";
    for (long long i: r_ans)
        std::cout << i + 1 - m << " ";
    return 0;
}
