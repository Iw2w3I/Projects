#include <iostream>
#include <vector>

//https://codeforces.com/group/PVbQ8eK2T4/contest/347382/problem/H
//complexity L*N
    
void StableCountSort(std::vector<long long int>& v, std::vector<long long int>& byte, std::vector<long long int>& cnt,
                     std::vector<long long int>& ans, std::vector<long long int>& num) {
    long long int i;
    for (i = 0;i < v.size();++i) {
        byte[i] = num[i] % 256;
    }
    for (i = 0;i < cnt.size();++i) { cnt[i] = 0; }
    for (i = 0;i < byte.size();++i) { ++cnt[byte[i]]; }
    for (i = 1;i < cnt.size();++i) { cnt[i] += cnt[i - 1]; }
    for (i = byte.size() - 1;i >= 0;--i) { ans[--cnt[byte[i]]] = v[i]; }
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    long long int n;
    std::cin >> n;
    std::vector<long long int> v(n);
    for (long long int i = 0;i < n;++i) {
        std::cin >> v[i];
    }
    std::vector<long long int> cnt(256);
    std::vector<long long int> byte(n);
    std::vector<long long int> ans = v;
    std::vector<long long int> num = v;
    for (long long int i = 0;i < 8;++i) {
        for (long long int j = 0;j < n;++j) { num[j] = v[j] >> 8 * i; }
        StableCountSort(v, byte, cnt, ans, num);
        for (long long int j = 0;j < n;++j) { v[j] = ans[j]; }
    }
    for (long long int i = 0;i < n;++i) {
        std::cout << ans[i] << " ";
    }
    return 0;
}
