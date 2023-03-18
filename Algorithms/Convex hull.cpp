#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <iomanip>
 
//https://codeforces.com/group/PVbQ8eK2T4/contest/410459/problem/F
//complexity N
 
const long long presicion = 0.0000001;
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////// - Point
struct Point {
    long long x;
    long long y;
 
    Point() = default;
    Point(long long x, long long y): x(x), y(y) {};
    Point(const Point& p) = default;
    Point& operator=(const Point& p) = default;
    Point& operator+=(const Point& p) {
        x += p.x;
        y += p.y;
        return *this;
    }
    Point& operator-=(const Point& p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }
    Point& operator*=(long long k) {
        x *= k;
        y *= k;
        return *this;
    }
    bool operator==(const Point& p) const {
        return (x == p.x && y == p.y);
    }
    bool closest(const Point& p1, const Point& p2) const {
        return (p1.x - x) * (p1.x - x) + (p1.y - y) * (p1.y - y) < (p2.x - x) * (p2.x - x) + (p2.y - y) * (p2.y - y);
    }
};
 
Point operator+(const Point& p1, const Point& p2) {
    return {p1.x + p2.x, p1.y + p2.y};
}
Point operator-(const Point& p1, const Point& p2) {
    return {p1.x - p2.x, p1.y - p2.y};
}
Point operator-(const Point& p, long long k) {
    return {p.x * k, p.y * k};
}
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.x << ' ' << p.y;
    return os;
}
 
long long dist(const Point& c1, const Point& c2) {
    return std::sqrt((c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y));
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////// - Vect
class Vect {
private:
    Point p;
public:
    Vect() = default;
    explicit Vect(long long px, long long py): p(px, py) {};
    explicit Vect(Point p): p(p) {};
    explicit Vect(long long ax, long long ay, long long bx, long long by): p(bx - ax, by - ay) {};
    long long length() const {
        return dist({0, 0}, p);
    }
    Point get_p() const {
        return p;
    }
    Vect& operator+=(const Vect& v) {
        p += v.p;
        return *this;
    }
    Vect& operator-=(const Vect& v) {
        p -= v.p;
        return *this;
    }
};
 
Vect operator+(const Vect& v1, const Vect& v2) {
    return Vect(v1.get_p() + v2.get_p());
}
Vect operator-(const Vect& v1, const Vect& v2) {
    return Vect(v1.get_p() - v2.get_p());
}
std::ostream& operator<<(std::ostream& os, const Vect& v) {
    os << v.get_p();
    return os;
}
 
long long dot(const Vect& v1, const Vect& v2) {
    return v1.get_p().x * v2.get_p().x + v1.get_p().y * v2.get_p().y;
}
long long cross(const Vect& v1, const Vect& v2) {
    return v1.get_p().x * v2.get_p().y - v1.get_p().y * v2.get_p().x;
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////// - Line
class Line {
private:
    long long a, b, c;
public:
    Line() = default;
    explicit Line(long long a, long long b, long long c): a(a), b(b), c(c) {};
    explicit Line(const Point& p1, const Point p2) {
        if (p1 == p2) {
            throw std::exception();
        }
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        c = p1.x * p2.y - p1.y * p2.x;
    }
    long long get_a() const {
        return a;
    }
    long long get_b() const {
        return b;
    }
    long long get_c() const {
        return c;
    }
    Vect normal() const {
        return Vect(-b, a);
    }
};
 
long long dist(const Point& p, const Line& l) {
    return (l.get_a() * p.x + l.get_b() * p.y + l.get_c()) / std::sqrt(l.get_a() * l.get_a() + l.get_b() * l.get_b());
}
 
bool parallel(const Line& l1, const Line& l2) {
    return l1.get_a() * l2.get_b() - l1.get_b() * l2.get_a() == 0;
}
 
long long dist(const Line& l1, const Line& l2) {
    if (!parallel(l1, l2)) {
        return 0;
    }
    long long k = (l2.get_a() == 0 ? l1.get_b() / l2.get_b() : l1.get_a() / l2.get_a());
    return std::abs(l1.get_c() - l2.get_c() * k) / std::sqrt(l1.get_a() * l1.get_a() + l1.get_b() * l1.get_b());
}
 
Point intersection(const Line& l1, const Line& l2) {
    return {(l1.get_b() * l2.get_c() - l1.get_c() * l2.get_b()) / (l1.get_a() * l2.get_b() - l1.get_b() * l2.get_a()),
            (l1.get_c() * l2.get_a() - l1.get_a() * l2.get_c()) / (l1.get_a() * l2.get_b() - l1.get_b() * l2.get_a())};
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////// - Segment
class Segment {
private:
    Point a, b;
    Line l;
public:
    Segment() = default;
    explicit Segment(Point a, Point b): a(a), b(b) {
        if (a == b) {
            l = Line({0, 0}, a);
        } else {
            l = Line(a, b);
        }
    };
    explicit Segment(long long ax, long long ay, long long bx, long long by): a(ax, ay), b(bx, by) {
        if (a == b) {
            l = Line({0, 0}, a);
        } else {
            l = Line(a, b);
        }
    };
    Point get_a() const {
        return a;
    }
    Point get_b() const {
        return b;
    }
    Line get_l() const {
        return l;
    }
    long long length() const {
        return dist(a, b);
    }
    bool contains(Point p) const {
        return dist(a, p) + dist(b, p) - length() < presicion;
    }
};
 
bool intersect(const Segment& l1, const Segment& l2) {
    if (parallel(l1.get_l(), l2.get_l())) {
        return l1.contains(l2.get_a()) || l1.contains(l2.get_b()) || l2.contains(l1.get_a());
    }
    Point i = intersection(l1.get_l(), l2.get_l());
    return l1.contains(i) && l2.contains(i);
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////// - Hull
class Hull {
private:
    std::vector<Point> hull;
public:
    Hull() = default;
    explicit Hull(std::vector<Point>& points) {
        Point p = points[0];
        for (Point i: points) {
            if (p.x > i.x || (p.y > i.y && p.x == i.x)) {
                p = i;
            }
        }
        std::sort(points.begin(), points.end(), [&](const Point& p1, const Point& p2) {
            long long c = cross(Vect(p1 - p), Vect(p2 - p));
            return c > 0 || (c == 0 && p.closest(p1, p2));
        });
        for (Point i: points) {
            while (hull.size() >= 2 && cross(Vect(hull.back() - i),
                                             Vect(hull.back() - hull[hull.size() - 2])) <= 0) {
                hull.pop_back();
            }
            hull.push_back(i);
        }
    }
    Point& operator[](size_t i) {
        return hull[i];
    }
    const Point& operator[](size_t i) const {
        return hull[i];
    }
    size_t size() const {
        return hull.size();
    }
};
 
std::ostream& operator<<(std::ostream& os, const Hull& h) {
    os << h.size() << '\n';
    for (int i = 0; i < h.size(); ++i) {
        os << h[i].x << " " << h[i].y << '\n';
    }
    return os;
}
 
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    int n;
    std::cin >> n;
    std::vector<Point> v(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> v[i].x >> v[i].y;
    }
    std::cout << Hull(v);
    return 0;
}
