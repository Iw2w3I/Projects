#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

//https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/L
//complexity N

void dfs(long long v, long long p, std::vector<long long>& tin, std::vector<long long>& tout,
         std::vector<std::vector<long long>>& g, std::vector<std::string>& color,
         std::vector<long long>& parent, long long& timer, std::vector<long long>& comp, long long& counter) {
    parent[v] = p;
    tin[v] = timer++;
    color[v] = "GREY";
    for (long long t: g[v]) {
        if (color[t] != "WHITE")
            continue;
        dfs(t, v, tin, tout, g, color, parent, timer, comp, counter);
    }
    color[v] = "BLACK";
    tout[v] = timer++;
    comp[v] = counter;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    long long n, m, a, b;
    std::cin >> n >> m;
    std::vector<long long> lamp(n);
    std::vector<std::vector<long long>> g(n, std::vector<long long> ());
    for (long long i = 0; i < n; ++i)
        std::cin >> lamp[i];
    for (long long i = 0; i < m; ++i) {
        std::cin >> a;
        for (long long j = 0; j < a; ++j) {
            std::cin >> b;
            g[b - 1].push_back(i);
        }
    }
    std::vector<long long> tin(2 * m);
    std::vector<long long> tout(2 * m);
    std::vector<std::vector<long long>> knf(2 * m, std::vector<long long> ());
    std::vector<std::string> color(2 * m, "WHITE");
    std::vector<long long> parent(2 * m);
    std::vector<long long> comp(2 * m);
    long long timer = 0, counter = 1;
    for (long long i = 0; i < n; ++i) {
        if (lamp[i]) {
            knf[g[i][0]].push_back(g[i][1]);
            knf[g[i][0] + m].push_back(g[i][1] + m);
            knf[g[i][1]].push_back(g[i][0]);
            knf[g[i][1] + m].push_back(g[i][0] + m);
        } else {
            knf[g[i][0]].push_back(g[i][1] + m);
            knf[g[i][0] + m].push_back(g[i][1]);
            knf[g[i][1]].push_back(g[i][0] + m);
            knf[g[i][1] + m].push_back(g[i][0]);
        }
    }
    for (long long i = 0 ; i < 2 * m; ++i) {
        if (color[i] == "WHITE")
            dfs(i, -1, tin, tout, knf, color, parent, timer, comp, counter);
    }
    color = std::vector<std::string> (2 * m, "WHITE");
    std::vector<std::vector<long long>> knf2(2 * m, std::vector<long long> ());
    for (long long i = 0; i < 2 * m; ++i) {
        for (long long j = 0; j < knf[i].size(); ++j)
            knf2[knf[i][j]].push_back(i);
    }
    std::vector<std::pair<long long, long long>> sorted(2 * m);
    for (long long i = 0; i < 2 * m; ++i)
        sorted[i] = {tout[i], i};
    std::sort(sorted.rbegin(), sorted.rend());
    for (std::pair<long long, long long> i: sorted) {
        if (color[i.second] != "WHITE")
            continue;
        dfs(i.second, -1, tin, tout, knf2, color, parent, timer, comp, counter);
        ++counter;
    }
    for (long long i = 0; i < m; ++i)
        if (comp[i] == comp[i + m]) {
            std::cout << "NO";
            return 0;
        }
    std::cout << "YES";
    return 0;
}
