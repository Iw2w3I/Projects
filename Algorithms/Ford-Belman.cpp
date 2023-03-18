#include<iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <string>

//https://codeforces.com/group/PVbQ8eK2T4/contest/377095/problem/D
//complexity N*M

void dfs(long long v, long long p, std::vector<long long>& tin, std::vector<long long>& tout,
         std::vector<std::vector<std::pair<long long, long long>>>& g, std::vector<std::string>& color,
         std::vector<long long>& parent, long long& timer) {
    parent[v] = p;
    tin[v] = timer++;
    color[v] = "GREY";
    for (std::pair<long long, long long> t: g[v]) {
        if (color[t.second] != "WHITE")
            continue;
        dfs(t.second, v, tin, tout, g, color, parent, timer);
    }
    color[v] = "BLACK";
    tout[v] = timer++;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    long long m, n, inp, inf = std::numeric_limits<long long>::max() / 10;
    std::cin >> n >> m >> inp;
    --inp;
    std::vector<std::vector<std::pair<long long, long long>>> g(n);
    std::vector<std::vector<std::pair<long long, long long>>> g_flipped(n);
    long long a, b, w;
    for (long long i = 0; i < m; ++i) {
        std::cin >> a >> b >> w;
        g[a - 1].push_back({w, b - 1});
        g_flipped[b - 1].push_back({w, a - 1});
    }
    std::vector<std::vector<long long>> dp(n + 1, std::vector<long long> (n, inf));
    dp[0][inp] = 0;
    for (long long k = 0; k < n; ++k)
        for (long long i = 0; i < n; ++i) {
            long long mi = inf;
            for (auto u: g_flipped[i])
                if (mi > dp[k][u.second] + u.first && dp[k][u.second] != inf)
                    mi = dp[k][u.second] + u.first;
            dp[k + 1][i] = std::min(dp[k][i], mi);
        }
    std::vector<long long> ans(n);
    for (long long i = 0; i < n; ++i) {
        if (dp[n][i] < dp[n - 1][i]) {
            long long timer = 0;
            std::vector<long long> tin(n);
            std::vector<long long> tout(n);
            std::vector<std::string> color(n, "WHITE");
            std::vector<long long> parent(n);
            dfs(i, -1, tin, tout, g, color, parent, timer);
            for (long long j = 0; j < n; ++j)
                if (color[j] == "BLACK")
                    ans[j] = -inf;
        }
    }
    for (long long i = 0; i < n; ++i) {
        if (ans[i] == -inf)
            std::cout << '-' << std::endl;
        else
        if (dp[n - 1][i] == inf)
            std::cout << '*' << std::endl;
        else
            std::cout << dp[n - 1][i] << std::endl;
    }
    return 0;
}
