#include <iostream>
#include <algorithm>
#include <vector>

//https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/D
//complexity N

bool dfs(long long v, long long p, std::vector<long long>& tin, std::vector<long long>& tout,
         std::vector<std::vector<long long>>& g, std::vector<std::string>& color,
         std::vector<long long>& parent, long long& timer, bool& possible) {
    parent[v] = p;
    tin[v] = timer++;
    color[v] = "GREY";
    for (long long t: g[v]) {
        if (color[t] == "GREY") {
            possible = false;
            break;
        }
        if (color[t] != "WHITE")
            continue;
        possible = dfs(t, v, tin, tout, g, color, parent, timer, possible);
    }
    color[v] = "BLACK";
    tout[v] = timer++;
    return possible;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    long long n, m, a, b;
    std::cin >> n >> m;
    std::vector<long long> tin(n);
    std::vector<long long> tout(n);
    std::vector<std::vector<long long>> g(n, std::vector<long long> ());
    std::vector<std::string> color(n, "WHITE");
    std::vector<long long> parent(n);
    long long timer = 0;
    bool possible = true;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b;
        g[a - 1].push_back(b - 1);
    }
    for (long long i = 0 ; i < n; ++i) {
        if (color[i] != "WHITE")
            continue;
        if (!dfs(i, -1, tin, tout, g, color, parent, timer, possible)) {
            std::cout << -1;
            return 0;
        }
    }
    std::vector<std::pair<long long, long long>> ans(n);
    for (long long i = 0 ; i < n; ++i)
        ans[i] = {tout[i], i};
    std::sort(ans.rbegin(), ans.rend());
    for (std::pair<long long, long long> i: ans)
        std::cout << i.second + 1 << " ";
    return 0;
}
