#include <iostream>
#include <algorithm>
#include <vector>

//https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/C
//complexity M+N

std::vector<long long>& dfs(long long v, long long p, std::vector<long long>& tin, std::vector<long long>& tout,
                            std::vector<std::vector<long long>>& g, std::vector<std::string>& color, std::vector<long long>& parent,
                            long long& timer, std::vector<long long>& ans) {
    parent[v] = p;
    tin[v] = timer++;
    color[v] = "GREY";
    for (long long t: g[v]) {
        if (color[t] == "GREY") {
            long long x = v;
            ans = {x};
            while (x != t) {
                x = parent[x];
                ans.push_back(x);
            }
            break;
        }
        if (color[t] != "WHITE")
            continue;
        ans = dfs(t, v, tin, tout, g, color, parent, timer, ans);
    }
    tout[v] = timer++;
    color[v] = "BLACK";
    return ans;
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
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b;
        g[a - 1].push_back(b - 1);
    }
    std::vector<long long> ans;
    for (long long i = 0 ; i < n; ++i) {
        if (color[i] != "WHITE")
            continue;
        ans = dfs(i, -1, tin, tout, g, color, parent, timer, ans);
        if (!ans.empty()) {
            std::cout << "YES" << std::endl;
            for (long long j = ans.size() - 1; j >= 0; --j)
                std::cout << ans[j] + 1 << " ";
            return 0;
        }
    }
    std::cout << "NO" << std::endl;
    return 0;
}
