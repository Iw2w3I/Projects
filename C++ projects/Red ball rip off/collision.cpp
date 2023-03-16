float distance_between_points(sf::Vector2f point_a, sf::Vector2f point_b) {
    return std::sqrt((point_a.x - point_b.x) * (point_a.x - point_b.x) +
                     (point_a.y - point_b.y) * (point_a.y - point_b.y));
}

struct Formula {
    float a, b, c;
};

class Collision_Point {
protected:
    sf::Vector2f point;
    bool is_jumpable;
public:
    Collision_Point(sf::Vector2f Point, bool Is_jumpable): point(Point) {}
    sf::Vector2f get_point() const {
        return point;
    }
    bool get_is_jumpable() const {
        return is_jumpable;
    }
    void set_is_jumpable(bool b) {
        is_jumpable = b;
    }
    void shift_pos(sf::Vector2f pos) {
        point += pos;
    }
    void draw(sf::RenderWindow& app) const {
        sf::CircleShape shape(3);
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(point);
        app.draw(shape);
    }
};

class Collision_Line {
protected:
    sf::Vector2f point_a, point_b;
    Collision_Point a, b;
    Formula f;
    char type;
    bool is_jumpable = false;

    void calculate_f() {
        f.a = point_a.y - point_b.y;
        f.b = -(point_a.x - point_b.x);
        f.c = -(f.a * point_a.x + f.b * point_a.y);
    }
public:
    Collision_Line(sf::Vector2f Point_a, sf::Vector2f Point_b, bool Is_jumpable, char Type):
            a(Point_a, Is_jumpable), b(Point_b, Is_jumpable),
            point_a(Point_a), point_b(Point_b), is_jumpable(Is_jumpable), type(Type) {
        calculate_f();
        a.set_is_jumpable(is_jumpable);
        b.set_is_jumpable(is_jumpable);
    }
    sf::Vector2f get_point_a() const {
        return point_a;
    }
    sf::Vector2f get_point_b() const {
        return point_b;
    }
    void shift_pos_a(sf::Vector2f pos) {
        a.shift_pos(pos);
    }
    void shift_pos_b(sf::Vector2f pos) {
        b.shift_pos(pos);
    }
    const Collision_Point& get_collision_point_a() const {
        return a;
    }
    const Collision_Point& get_collision_point_b() const {
        return b;
    }
    void set_is_jumpable(bool w) {
        is_jumpable = w;
        a.set_is_jumpable(w);
        b.set_is_jumpable(w);
    }
    bool get_is_jumpable() const {
        return is_jumpable;
    }
    char get_type() const {
        return type;
    }
    float get_angle() const {
        return std::atan((point_a.y - point_b.y) / (point_a.x - point_b.x));
    }
    float get_coord_y(float x) const {
        if (f.b != 0)
            return -(f.a * x + f.c) / f.b;
        return point_a.y;
    }
    bool is_point_inside(sf::Vector2f point) const {
        return (std::abs(distance_between_points(point_a, point)) +
                std::abs(distance_between_points(point_b, point)) <=
                std::abs(distance_between_points(point_a, point_b)) + 0.15);
    }
    float distance(sf::Vector2f point) const {
        return (f.a * point.x + f.b * point.y + f.c) / std::sqrt(f.a * f.a + f.b * f.b);
    }
    sf::Vector2f reflect(sf::Vector2f point, sf::Vector2f center) const {
        Formula f_copy = f, vertical = f;
        f_copy.c = -(f_copy.a * center.x + f_copy.b * center.y);
        vertical.a = f_copy.b;
        vertical.b = -f_copy.a;
        vertical.c = -(vertical.a * point.x + vertical.b * point.y);
        sf::Vector2f projection, new_point;
        if (f_copy.a == 0) {
            projection.x = point.x;
            projection.y = point_a.y;
        } else
        if (f_copy.b == 0) {
            projection.x = point_a.x;
            projection.y = point.y;
        } else {
            projection.x = -(f_copy.a * f_copy.c + vertical.a * vertical.c)
                           / (f_copy.a * f_copy.a + vertical.a * vertical.a);
            projection.y = -(f_copy.b * f_copy.c + vertical.b * vertical.c)
                           / (f_copy.a * f_copy.a + vertical.a * vertical.a);
        }
        new_point = projection + projection - point;
        return new_point;
    }
    bool positive_side(sf::Vector2f point) const {
        if (type == 'l')
            return point.x > point_a.x;
        if (type == 'r')
            return point.x < point_a.x;
        if (f.a * point.x + f.b * point.y + f.c > 0)
            return type == 'u';
        return type == 'd';
    }
    void shift_pos(sf::Vector2f pos) {
        point_a += pos;
        point_b += pos;
        a.shift_pos(pos);
        b.shift_pos(pos);
        calculate_f();
    }
    void shift_size(sf::Vector2f pos) {
        point_a -= pos;
        point_b += pos;
        a.shift_pos(-pos);
        b.shift_pos(pos);
        calculate_f();
    }
    void draw(sf::RenderWindow& app) const {
        sf::RectangleShape shape(sf::Vector2f(distance_between_points(point_a, point_b), 5));
        shape.setFillColor(sf::Color::Green);
        shape.rotate(get_angle() / acos(-1) * 180);
        shape.setPosition(point_a);
        app.draw(shape);
    }
};

class Collision_Circle {
protected:
    sf::Vector2f center;
    float radius;
public:
    Collision_Circle(sf::Vector2f Center, float Radius): center(Center), radius(Radius) {}
    bool collision(Collision_Line l) const {
        float dist_a = distance_between_points(center, l.get_point_a()),
                dist_b = distance_between_points(center, l.get_point_b());
        dist_a = dist_a * dist_a - radius * radius;
        dist_b = dist_b * dist_b - radius * radius;
        if (dist_a < 0)
            dist_a = 0;
        if (dist_b < 0)
            dist_b = 0;
        return (std::sqrt(dist_a) + std::sqrt(dist_b) <=
                std::abs(distance_between_points(l.get_point_a(), l.get_point_b())) + 0.15);
    }
    bool collision(Collision_Point a) const {
        return distance_between_points(a.get_point(), center) <= radius;
    }
    bool collision(Object* object) {
        bool ans = false;
        std::vector<Collision_Point> points = object->get_collision_points();
        for (auto &p: points)
            ans = ans || collision(p);
        std::vector<Collision_Line> lines = object->get_collision_lines();
        for (auto &l: lines)
            ans = ans || collision(l);
        return ans;
    }
    void set_pos(sf::Vector2f pos) {
        center = pos;
    }
    void shift_pos(sf::Vector2f pos) {
        center += pos;
    }
};

class Collision_Rect {
protected:
    Collision_Line up_l, down_l, left_l, right_l;
public:
    Collision_Rect(sf::Vector2f Up_left_point, sf::Vector2f Down_right_point, bool Is_jumpable):
            up_l(Up_left_point, {Down_right_point.x, Up_left_point.y}, Is_jumpable, 'u'),
            down_l({Up_left_point.x, Down_right_point.y},Down_right_point, false, 'd'),
            left_l({Up_left_point.x, Up_left_point.y + 1},
                   {Up_left_point.x, Down_right_point.y - 1}, false, 'l'),
            right_l({Down_right_point.x, Up_left_point.y + 1},
                    {Down_right_point.x, Down_right_point.y - 1}, false, 'r') {
        up_l.shift_pos_a({1, 1});
        up_l.shift_pos_b({-1, 1});
        down_l.shift_pos_a({1, -1});
        down_l.shift_pos_b({-1, -1});
    }
    sf::Vector2f get_center() const {
        return {(left_l.get_point_a().x + right_l.get_point_a().x) / 2,
                (up_l.get_point_a().y + down_l.get_point_a().y) / 2};
    }
    void shift_pos(sf::Vector2f pos) {
        up_l.shift_pos(pos);
        down_l.shift_pos(pos);
        left_l.shift_pos(pos);
        right_l.shift_pos(pos);
    }
    void shift_size(sf::Vector2f pos) {
        up_l.shift_size({pos.x, 0});
        up_l.shift_pos({0, -pos.y});
        down_l.shift_size({pos.x, 0});
        down_l.shift_pos({0, pos.y});
        left_l.shift_size({0, pos.y});
        left_l.shift_pos({-pos.x, 0});
        right_l.shift_size({0, pos.y});
        right_l.shift_pos({pos.x, 0});
        if (std::abs(left_l.get_point_a().x - right_l.get_point_a().x) < 30) {
            up_l.shift_size({-pos.x, 0});
            down_l.shift_size({-pos.x, 0});
            left_l.shift_pos({pos.x, 0});
            right_l.shift_pos({-pos.x, 0});
        }
        if (std::abs(up_l.get_point_a().y - down_l.get_point_a().y) < 30) {
            left_l.shift_size({0, -pos.y});
            right_l.shift_size({0, -pos.y});
            up_l.shift_pos({0, pos.y});
            down_l.shift_pos({0, -pos.y});
        }
    }
};