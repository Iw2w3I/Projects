#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <complex>

//https://codeforces.com/group/PVbQ8eK2T4/contest/405815/submission/178535453
//complexity N*logN

//inspired by https://e-maxx.ru/algo/export_fft_multiply

void fft (std::vector<std::complex<double>> & a, bool inverted) {
	int size = a.size(), j = 0, bit;
    for (int i = 1; i < size; ++i) {
	    bit = size >> 1;
        while (j >= bit) {
            j -= bit;
            bit >>= 1;
        }
		j += bit;
		if (i < j)
			swap (a[i], a[j]);
	}
    std::complex<double> e, rotate, a1, a2;
    for (int iter = 2; iter <= size; iter *= 2) {
        double angle = 2 * acos(-1) / iter * (1 - 2 * inverted);
        rotate = {cos(angle), sin(angle)};
        for (int i = 0; i < size; i += iter) {
            e = 1;
            for (int j = 0; j < iter / 2; ++j) {
                a1 = a[i + j];
                a2 = a[i + j + iter / 2];
                a[i + j] = a1 + a2 * e;
                a[i + j + iter / 2] = a1 - a2 * e;
                e *= rotate;
            }
        }
    }
	if (inverted) {
		for (int i = 0; i < size; ++i) {
			a[i] /= size;
        }
    }
}

std::vector<int> multiply (std::vector<int>& a, std::vector<int>& b) {
    std::vector<std::complex<double>> newa(a.begin(), a.end());
    std::vector<std::complex<double>> newb(b.begin(), b.end());
    int n = 1;
    while (n < a.size() + b.size()) {
        n <<= 1;
    }
    newa.resize(n);
    fft(newa, false);
    newb.resize(n);
    fft(newb, false);
    for (int i = 0; i < n; ++i) {
        newa[i] *= newb[i];
    }
    fft(newa, true);
    std::vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        if (newa[i].real() > 0) {
            ans[i] = static_cast<int>(newa[i].real() + 0.5);
        } else {
            ans[i] = static_cast<int>(newa[i].real() - 0.5);
        }
    }
    return ans;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m;
    std::cin >> n;
    std::vector<int> a(n + 1);
    for (int i = 0; i <= n; ++i) {
        std::cin >> a[n - i];
    }
    std::cin >> m;
    std::vector<int> b(m + 1);
    for (int i = 0; i <= m; ++i) {
        std::cin >> b[m - i];
    }
    auto ans = multiply(a, b);
    int i = ans.size() - 1;
    while (ans[i] == 0) {
        --i;
    }
    std::cout << i << " ";
    while (i >= 0) {
        std::cout << ans[i--] << " ";
    }
    return 0;
}
