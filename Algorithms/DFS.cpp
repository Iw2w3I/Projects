#include <iostream>
#include <algorithm>
#include <vector>

//https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/B
//complexity N

void dfs(long long v, long long p, std::vector<long long>& tin, std::vector<long long>& tout,
         std::vector<std::vector<long long>>& g, std::vector<bool>& used, long long& timer) {
    tin[v] = timer++;
    used[v] = true;
    for (long long t: g[v]) {
        if (used[t])
            continue;
        dfs(t, v, tin, tout, g, used, timer);
    }
    tout[v] = timer++;
}

void dfs(long long v, long long p, std::vector<long long>& tin, std::vector<long long>& tout, std::vector<std::vector<long long>>& g) {
    std::vector<bool> used(g.size());
    long long timer = 0;
    dfs(v, p, tin, tout, g, used, timer);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    int n;
    std::cin >> n;
    std::vector<long long> v(n);
    for (int i = 0; i < n; ++i)
        std::cin >> v[i];
    long long m, a, b;
    std::cin >> m;
    std::vector<long long> tin(n);
    std::vector<long long> tout(n);
    std::vector<std::vector<long long>> g(n, std::vector<long long> ());
    for (long long i = n - 1; i >= 0; --i) {
        if (v[i] != 0)
            g[v[i] - 1].push_back(i);
    }
    long long root = 0;
    for (long long i = 0; i < n; ++i)
        if (v[i] == 0) {
            root = i;
            break;
        }
    dfs(root, 0, tin, tout, g);
    for (long long i = 0; i < m; ++i) {
        std::cin >> a >> b;
        std::cout << (tin[a - 1] < tin[b - 1] && tout[a - 1] > tout[b - 1]) << std::endl;
    }
    return 0;
}
