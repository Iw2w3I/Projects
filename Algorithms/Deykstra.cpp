#include<iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <string>

//https://codeforces.com/group/PVbQ8eK2T4/contest/377095/problem/F
//complexity M*logN

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    long long m, n, inp, outp, inf = std::numeric_limits<long long>::max();
    std::cin >> n >> m >> inp >> outp;
    --inp;
    --outp;
    std::vector<std::vector<std::pair<long long, long long>>> g(n);
    long long a, b, w;
    for (long long i = 0; i < m; ++i) {
        std::cin >> a >> b >> w;
        g[a - 1].push_back({w, b - 1});
        g[b - 1].push_back({w, a - 1});
    }
    std::vector<long long> dist(n, inf);
    std::vector<bool> used(n, false);
    dist[inp] = 0;
    std::priority_queue<std::pair<long long, long long>, std::vector<std::pair<long long, long long>>,
            std::greater<>> q;
    q.push({0, inp});
    while (!q.empty()) {
        long long v = q.top().second;
        q.pop();
        if (used[v])
            continue;
        used[v] = true;
        for (std::pair<long long, long long> e: g[v]) {
            if (dist[e.second] > dist[v] + e.first) {
                dist[e.second] = dist[v] + e.first;
                q.push({dist[e.second], e.second});
            }
        }
    }
    if (dist[outp] == inf)
        std::cout << -1;
    else
        std::cout << dist[outp];
    return 0;
}
