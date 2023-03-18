#include <iostream>
#include <unordered_map>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <vector>
#include <algorithm>
#pragma optimize("O3")
#pragma GCC optimize("Ofast,no-stack-protector")

//https://codeforces.com/group/PVbQ8eK2T4/contest/377095/problem/K

short size = 4, size_square = size * size;

uint64_t hash(std::vector<uint64_t>& a) {
    uint64_t ans = 0;
    for (short i = 0; i < a.size(); ++i)
        ans += (a[i] << (size * i));
    return ans;
}

class Table {
private:
    uint64_t pos;
    short null_pos;
public:
    Table(std::vector<uint64_t>& a) {
        pos = hash(a);
        null_pos = std::find(a.begin(), a.end(), 0) - a.begin();
    }
    Table(uint64_t a): pos(a) {
        for (short i = 0; i < size_square; ++i) {
            if (a % size_square == 0) {
                null_pos = i;
                return;
            }
            a >>= 4;
        }
    }
    short get_value(short index) {
        return (pos >> (size * index)) % (size_square);
    }
    void swap_value(short index1, short index2) {
        uint64_t copy1 = get_value(index1), copy2 = get_value(index2);
        if (copy1 > copy2) {
            pos += ((copy1 - copy2) << (size * index2));
            pos -= ((copy1 - copy2) << (size * index1));
        } else {
            pos -= ((copy2 - copy1) << (size * index2));
            pos += ((copy2 - copy1) << (size * index1));
        }
    }
    short get_null() {
        return null_pos;
    }
    void set_null(short null) {
        null_pos = null;
    }
    uint64_t get_pos() {
        return pos;
    }
    bool operator==(Table& a) const {
        return pos == a.pos;
    }
    bool operator!=(Table& a) const {
        return pos != a.pos;
    }
    void move_pos(char c) {
        if (c == 'U') {
            swap_value(null_pos, null_pos - size);
            null_pos -= size;
        }
        if (c == 'D') {
            swap_value(null_pos, null_pos + size);
            null_pos += size;
        }
        if (c == 'L') {
            swap_value(null_pos, null_pos - 1);
            null_pos -= 1;
        }
        if (c == 'R') {
            swap_value(null_pos, null_pos + 1);
            null_pos += 1;
        }
    }
};

bool valid_pos(Table& pos) {
    short invar = pos.get_null() / size + 1;
    for (short i = 0; i < size_square; ++i) {
        if (pos.get_null() != i)
            for (short j = 0; j < i; ++j)
                if (pos.get_value(j) > pos.get_value(i))
                    ++invar;
    }
    return invar % 2 == 0;
}

short get_value(uint64_t& pos, short index) {
    return (pos >> (size * index)) % (size_square);
}

void swap_value(uint64_t& pos, short index1, short index2) {
    uint64_t copy1 = get_value(pos, index1), copy2 = get_value(pos, index2);
    if (copy1 > copy2) {
        pos += ((copy1 - copy2) << (size * index2));
        pos -= ((copy1 - copy2) << (size * index1));
    } else {
        pos -= ((copy2 - copy1) << (size * index2));
        pos += ((copy2 - copy1) << (size * index1));
    }
}

std::vector<std::pair<uint64_t, char>> next_pos(Table& u) {
    std::vector<std::pair<uint64_t, char>> ans;
    short null = u.get_null();
    if (null % size != 0) {
        uint64_t copy = u.get_pos();
        swap_value(copy, null, null - 1);
        ans.push_back({copy, 'R'});
    }
    if (null >= size) {
        uint64_t copy = u.get_pos();
        swap_value(copy, null, null - size);
        ans.push_back({copy, 'D'});
    }
    if (null % size != size - 1) {
        uint64_t copy = u.get_pos();
        swap_value(copy, null, null + 1);
        ans.push_back({copy, 'L'});
    }
    if (null < size_square - size) {
        uint64_t copy = u.get_pos();
        swap_value(copy, null, null + size);
        ans.push_back({copy, 'U'});
    }
    return ans;
}

double distance(Table& pos) {
    uint64_t ans = 0;
    for (short i = 0; i < size_square; ++i) {
        if (pos.get_null() == i)
            continue;
        uint64_t index = pos.get_value(i) - 1;
        ans += abs(i % 4 - index % 4) + abs(i / 4 - index / 4);
    }
    return ans;
}

short row_conflcts(const short* v, short index) {
    short ans = 0;
    for (short i = 0; i < size; ++i)
        if (v[i] > 0)
            for (short j = 0; j < i; ++j)
                if (v[i] < v[j] && (v[i] / size == index) && (v[j] / size == index))
                    ++ans;
    return ans;
}

short column_conflcts(const short* v, short index) {
    short ans = 0;
    for (short i = 0; i < size; ++i)
        if (v[i] > 0)
            for (short j = 0; j < i; ++j)
                if (v[i] < v[j] && (v[i] % size == index) && (v[j] % size == index))
                    ++ans;
    return ans;
}

double linear_conflict(Table& pos) {
    short row[size], column[size], conflict = 0;
    for (short i = 0; i < size; ++i) {
        for (short j = 0; j < size; ++j) {
            row[j] = pos.get_value(i * size + j) - 1;
            column[j] = pos.get_value(j * size + i) - 1;
        }
        conflict += row_conflcts(row, i);
        conflict += column_conflcts(column, i);
    }
    return conflict;
}

short corner_conflict(Table& pos) {
    short ans = 0;
    if ((pos.get_value(2) == 3 && pos.get_value(3) != 4 && pos.get_value(7) == 8) ||
        (pos.get_value(0) != 1 && pos.get_value(1) == 2 && pos.get_value(4) == 5) ||
        (pos.get_value(8) == 9 && pos.get_value(13) == 14 && pos.get_value(12) != 13))
        ans += 2;
    return ans;
}

double super_constant = 1.095;
short conflict(Table& pos) {
    return super_constant * (distance(pos) + 2 * linear_conflict(pos) + corner_conflict(pos));
}

int main() {
    std::ios::sync_with_stdio(false); //solves 15 puzzle (empty square is 0)
    std::cin.tie(0);
    std::cout.tie(0);
    std::vector<uint64_t> a(size_square);
    for (short i = 0; i < a.size(); ++i)
        std::cin >> a[i];
    Table start(a);
    for (short i = 0; i < a.size(); ++i)
        a[i] = i + 1;
    a[15] = 0;
    Table finish(a);
    if (!valid_pos(start)) {
        std::cout << -1;
        return 0;
    }
    std::unordered_map<uint64_t, std::pair<int, char>> m;
    std::unordered_set<uint64_t> used;
    std::priority_queue<std::pair<int, uint64_t>, std::vector<std::pair<int, uint64_t>>, std::greater<>> q;
    m[start.get_pos()] = {1, 'n'};
    q.push({0, start.get_pos()});
    while (!q.empty()) {
        Table u = q.top().second;
        q.pop();
        used.insert(u.get_pos());
        if (u == finish)
            break;
        for (std::pair<Table, char> e: next_pos(u)) {
            if (m[e.first.get_pos()].first > m[u.get_pos()].first + 1 || m[e.first.get_pos()].first == 0) {
                m[e.first.get_pos()] = {m[u.get_pos()].first + 1, e.second};
                if (!used.contains(e.first.get_pos()))
                    q.push({m[e.first.get_pos()].first + conflict(e.first), e.first.get_pos()});
            }
        }

    }
    std::cout << m[finish.get_pos()].first - 1 << '\n';
    std::vector<char> ans;
    while (finish != start) {
        ans.push_back(m[finish.get_pos()].second);
        finish.move_pos(m[finish.get_pos()].second);
    }
    for (short i = 0; i < ans.size(); ++i)
        std::cout << ans[ans.size() - i - 1];
    return 0;
}
