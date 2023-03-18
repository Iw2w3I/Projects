#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
#include <queue>
#include <set>
#include <map>
 
//https://codeforces.com/group/PVbQ8eK2T4/contest/400596/problem/L
//complexity N
 
int alpha = 27;
 
class Suf_tree;
 
class Suf_automate {
private:
    friend Suf_tree;
 
    struct node {
        std::map<int, int> to;
        int link = -1, len = 0, end = 0;
    };
    int last = 0, curr;
    std::vector<node> t;
public:
    Suf_automate() {
        t.push_back(node());
    }
 
    void add(int c) {
        curr = t.size();
        t.push_back(node());
        t[curr].len = t[last].len + 1;
        t[curr].end = t[last].end + 1;
        int p = last;
        while (p != -1 && !t[p].to.count(c)) {
            t[p].to[c] = curr;
            p = t[p].link;
        }
        if (p == -1) {
            t[curr].link = 0;
            last = curr;
            return;
        }
        int q = t[p].to[c];
        if (t[q].len == t[p].len + 1) {
            t[curr].link = q;
            last = curr;
            return;
        }
        int newq = t.size();
        t.push_back(node());
        t[newq].len = t[p].len + 1;
        t[newq].end = t[q].end;
        while (p != -1 && t[p].to[c] == q) {
            t[p].to[c] = newq;
            p = t[p].link;
        }
        t[newq].to = t[q].to;
        t[newq].link = t[q].link;
        t[curr].link = t[q].link = newq;
        last = curr;
    }
};
 
 
class Suf_tree {
private:
    struct node {
        std::map<int, int> to;
        int begin = -1, len = 0;
    };
    std::vector<node> t;
    Suf_automate a;
    std::string str;
public:
    Suf_tree(std::string& str): str(str) {
        for (int i = str.size() - 1; i >= 0; --i) {
            if (str[i] == '$') {
                a.add('z' - 'a' + 1);
            } else {
                a.add(str[i] - 'a');
            }
        }
        t.resize(a.t.size());
        for (int i = 1; i < a.t.size(); ++i) {
            calc(i);
        }
    }
 
    void calc(int v) {
        int link = a.t[v].link;
        t[v].begin = a.t[link].len + str.size() - a.t[v].end;
        t[v].len = a.t[v].len - a.t[link].len;
        t[link].to[str[t[v].begin]] = v;
    }
 
    int get_size() {
        return t.size();
    }
 
    void print(int v, int& count) {
        int ans = count;
        for (auto p: t[v].to) {
            std::cout << ans << " " << t[p.second].begin << " " << t[p.second].begin + t[p.second].len << '\n';
            print(p.second, ++count);
        }
    }
};
 
 
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::string str;
    std::cin >> str;
    Suf_tree t(str);
    std::cout << t.get_size() << '\n';
    int count = 0;
    t.print(0, count);
    return 0;
}
