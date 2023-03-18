#include <iostream>
#include <vector>

//https://codeforces.com/group/PVbQ8eK2T4/contest/374347/problem/C
//complexity N^3

int main() {
    long long n;
    std::cin >> n;
    std::vector<std::vector<long long>> g(n, std::vector<long long>(n));
    for (long long i = 0; i < n; ++i)
        for (long long j = 0; j < n; ++j)
            std::cin >> g[i][j];
    for (long long k = 0; k < n; ++k)
        for (long long i = 0; i < n; ++i)
            for (long long j = 0; j < n; ++j)
                g[i][j] = std::min(g[i][j], g[i][k] + g[k][j]);
    for (long long i = 0; i < n; ++i) {
        for (long long j = 0; j < n; ++j)
            std::cout << g[i][j] << " ";
        std::cout << std::endl;
    }
    return 0;
}
