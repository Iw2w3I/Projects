#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <iomanip>
 
//https://codeforces.com/group/PVbQ8eK2T4/contest/410459/problem/L
//complexity N+M
 
const long double presicion = 0.0000001;
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////// - Point
struct Point {
    long double x;
    long double y;
 
    Point() = default;
    Point(long double x, long double y): x(x), y(y) {};
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
    Point& operator*=(long double k) {
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
Point operator-(const Point& p, long double k) {
    return {p.x * k, p.y * k};
}
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.x << ' ' << p.y;
    return os;
}
 
long double dist(const Point& c1, const Point& c2) {
    return std::sqrt((c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y));
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////// - Vect
class Vect {
private:
    Point p;
public:
    Vect() = default;
    explicit Vect(long double px, long double py): p(px, py) {};
    explicit Vect(Point p): p(p) {};
    explicit Vect(long double ax, long double ay, long double bx, long double by): p(bx - ax, by - ay) {};
    explicit Vect(Point p1, Point p2): p(p2 - p1) {};
    long double length() const {
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
 
long double dot(const Vect& v1, const Vect& v2) {
    return v1.get_p().x * v2.get_p().x + v1.get_p().y * v2.get_p().y;
}
long double cross(const Vect& v1, const Vect& v2) {
    return v1.get_p().x * v2.get_p().y - v1.get_p().y * v2.get_p().x;
}
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////// - Line
class Line {
private:
    long double a, b, c;
public:
    Line() = default;
    explicit Line(long double a, long double b, long double c): a(a), b(b), c(c) {};
    explicit Line(const Point& p1, const Point p2) {
        if (p1 == p2) {
            throw std::exception();
        }
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        c = p1.x * p2.y - p1.y * p2.x;
    }
    long double get_a() const {
        return a;
    }
    long double get_b() const {
        return b;
    }
    long double get_c() const {
        return c;
    }
    Vect normal() const {
        return Vect(-b, a);
    }
};
 
long double dist(const Point& p, const Line& l) {
    return (l.get_a() * p.x + l.get_b() * p.y + l.get_c()) / std::sqrt(l.get_a() * l.get_a() + l.get_b() * l.get_b());
}
 
bool parallel(const Line& l1, const Line& l2) {
    return l1.get_a() * l2.get_b() - l1.get_b() * l2.get_a() == 0;
}
 
long double dist(const Line& l1, const Line& l2) {
    if (!parallel(l1, l2)) {
        return 0;
    }
    long double k = (l2.get_a() == 0 ? l1.get_b() / l2.get_b() : l1.get_a() / l2.get_a());
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
    explicit Segment(long double ax, long double ay, long double bx, long double by): a(ax, ay), b(bx, by) {
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
    long double length() const {
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////// - Polygon
 
class Polygon {
private:
    std::vector<Point> polygon;
public:
    Polygon() = default;
    explicit Polygon(std::vector<Point>& points): polygon(points) {};
    bool convex() {
        if (polygon.size() < 3) {
            return true;
        }
        int count1 = 0, count_1 = 0;
        for (int i = 0; i < size(); ++i) {
            double c = cross(Vect(polygon[i], polygon[(i + 1) % size()]),
                             Vect(polygon[(i + 1) % size()], polygon[(i + 2) % size()]));
            if (c > 0) {
                ++count1;
            } else {
                if (c < 0) {
                    ++count_1;
                }
            }
        }
        return count1 == 0 || count_1 == 0;
    }
    int size() const {
        return polygon.size();
    }
    Point& operator[](size_t i) {
        return polygon[i];
    }
    const Point& operator[](size_t i) const {
        return polygon[i];
    }
    void generalize() {
        if (size() < 3) {
            return;
        }
        for (int i = 0; i < size(); ++i) {
            Vect v1(polygon[i], polygon[(i - 1) % size()]);
            Vect v2(polygon[i], polygon[(i + 1) % size()]);
            if ((cross(v1, v2) == 0) && ((v1.get_p().x * v2.get_p().x <= 0) || (v1.get_p().y * v2.get_p().y <= 0))) {
                polygon.erase(polygon.begin() + i);
            }
        }
    }
    int left_point() const {
        int p = 0;
        for (int i = 1; i < polygon.size(); ++i) {
            if (polygon[p].x > polygon[i].x || (polygon[p].y < polygon[i].y && polygon[p].x == polygon[i].x)) {
                p = i;
            }
        }
        return p;
    }
    bool contains(const Point p) {
        int intersections = 0;
        Point a, b;
        for (int i = 0; i < polygon.size(); ++i) {
            a = polygon[i];
            b = polygon[(i + 1) % polygon.size()];
            if (Segment(a, b).contains(p)) {
                return true;
            }
            if (b.y < a.y) {
                std::swap(a, b);
            }
            if (b.y <= p.y || a.y > p.y) {
                continue;
            }
            if (cross(Vect(p - a), Vect(b - a)) < 0) {
                ++intersections;
            }
        }
        return intersections % 2 == 1;
    }
    bool contains_in_convexed(const Point p) {
        if (p.x < polygon[0].x) {
            return false;
        }
        if (size() < 3) {
            return Segment(polygon[0], polygon.back()).contains(p);
        }
        int l = 1, r = size() - 1, m;
//        if (polygon[l].y > polygon[r].y) {
//            std::swap(l, r);
//        }
        Vect v(0, 1), vp(polygon[0], p);
        while (std::abs(r - l) > 1) {
            m = (r + l) / 2;
            Vect vm(polygon[0], polygon[m]);
            long double delta = dot(vm, v) * vp.length() - dot(vp, v) * vm.length();
            if (std::abs(delta) < presicion) {
                return Segment(polygon[0], polygon[m]).contains(p);
            }
            if (delta < 0) {
                l = m;
            } else {
                r = m;
            }
        }
        std::vector<Point> tr = {polygon[0], polygon[l], polygon[r]};
        return Polygon(tr).contains(p);
    }
};
 
Polygon operator+(const Polygon& p1, const Polygon& p2) {
    int l1 = p1.left_point(), l2 = p2.left_point();
    std::vector<Point> new_p;
    new_p.push_back(p1[l1] + p2[l2]);
    int i = l1, j = l2, i_inc = 1, j_inc = 1;
//    if (p1[(l1 + 1) % p1.size()].y > p1[(l1 - 1 + p1.size()) % p1.size()].y) {
//        i_inc = -1;
//    }
//    if (p2[(l2 + 1) % p2.size()].y > p2[(l2 - 1 + p2.size()) % p2.size()].y) {
//        j_inc = -1;
//    }
    do {
        int next_i = (i + i_inc + p1.size()) % p1.size(), next_j = (j + j_inc + p2.size()) % p2.size();
        if (cross(Vect(p1[i], p1[next_i]), Vect(p2[j], p2[next_j])) >= 0) {
            new_p.push_back(new_p.back() + p1[next_i] - p1[i]);
            i = next_i;
        } else {
            new_p.push_back(new_p.back() + p2[next_j] - p2[j]);
            j = next_j;
        }
    } while (i != l1 || j != l2);
    new_p.pop_back();
    return Polygon(new_p);
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
            long double c = cross(Vect(p1 - p), Vect(p2 - p));
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
    int n1, n2, n3;
    std::cin >> n1;
    std::vector<Point> v1(n1);
    for (int i = 0; i < n1; ++i) {
        std::cin >> v1[i].x >> v1[i].y;
    }
    std::cin >> n2;
    std::vector<Point> v2(n2);
    for (int i = 0; i < n2; ++i) {
        std::cin >> v2[i].x >> v2[i].y;
    }
    std::cin >> n3;
    std::vector<Point> v3(n3);
    for (int i = 0; i < n3; ++i) {
        std::cin >> v3[i].x >> v3[i].y;
    }
    Polygon p1(v1), p2(v2), p3(v3);
    Polygon sum = p1 + p2 + p3;
    sum.generalize();
    int q;
    std::cin >> q;
    for (int i = 0; i < q; ++i) {
        Point p;
        std::cin >> p.x >> p.y;
        if (sum.contains_in_convexed(Point(p.x * 3, p.y * 3))) {
            std::cout << "YES" << '\n';
        } else {
            std::cout << "NO" << '\n';
        }
    }
    return 0;
}
