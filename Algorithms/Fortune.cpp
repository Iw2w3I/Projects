#include <iostream>
#include <vector>
#include <cmath>
#include <deque>
#include <queue>
#include <SFML/Graphics.hpp>

// Voronoi diagramm, Fortune's algorithm N*log N, https://www.cs.hmc.edu/~mbrubeck/voronoi.html

int appWidth = 600, appHeight = 600;

class Wall {
private:
    float x, y;
    sf::CircleShape shape;
    void setShape() {
        float r = 3;
        shape.setRadius(r);
        shape.setPointCount(8);
        shape.setPosition(x - r / 2.f, y - r / 2.f);
        shape.setFillColor(sf::Color::White);
    }
public:
    Wall() = delete;
    Wall(float x, float y): x(x), y(y) {
        setShape();
    }
    Wall(sf::Vector2i coords): x(coords.x), y(coords.y) {
        setShape();
    }
    Wall(sf::Vector2f coords): x(coords.x), y(coords.y) {
        setShape();
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }
    sf::Vector2f v2f() const {
        return {x, y};
    }
    sf::Vector2f shapePos() const {
        return shape.getPosition();
    }
    Wall operator+(const Wall& wall) const {
        return {x + wall.x, y + wall.y};
    }
    Wall operator-(const Wall& wall) const {
        return {x - wall.x, y - wall.y};
    }
    void operator+=(const Wall& wall) {
        x += wall.x;
        y += wall.y;
    }
    void operator-=(const Wall& wall) {
        x -= wall.x;
        y -= wall.y;
    }
};

class Segment {
private:
    sf::Vector2f a, b;
    sf::RectangleShape shape;
    float length() const {
        sf::Vector2f delta = a - b;
        return std::sqrt(delta.x * delta.x + delta.y * delta.y);
    }
    double angle() const {
        return a.x == b.x ? std::acos(-1) / 2 : std::atan((b.y - a.y) / (b.x - a.x));
    }
    sf::Vector2f shapePos() const {
        sf::Vector2f af = a + a - Wall(a).shapePos();
        sf::Vector2f bf = b + b - Wall(b).shapePos();
        return (af.x < bf.x || af.x == bf.x && af.y < bf.y) ? af : bf;
    }
    void setShape() {
        shape.setSize({length(), 1});
        shape.setRotation(angle() * 180 / std::acos(-1));
        shape.setPosition(shapePos());
        shape.setFillColor(sf::Color::White);
    }
public:
    Segment() = delete;
    Segment(sf::Vector2f point1, sf::Vector2f point2): a(point1), b(point2) {
        setShape();
    }
    Segment(Wall& point1, Wall& point2): a(point1.v2f()), b(point2.v2f()) {
        setShape();
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }
};


struct ParabolaSegment;
struct Parabola;
struct CircleEvent;

std::vector<ParabolaSegment*> output;
Parabola* root = nullptr;

struct ParabolaSegment {
    sf::Vector2f startPoint, finishPoint;
    bool complete = false;
    ParabolaSegment(sf::Vector2f point): startPoint(point), finishPoint(0,0) {
        output.push_back(this);
    };
    void finish(sf::Vector2f point) {
        if (complete) {
            return;
        }
        finishPoint = point;
        complete = true;
    }
};

struct Parabola {
    sf::Vector2f focus;
    Parabola *prev, *next;
    ParabolaSegment *seg1, *seg2;
    CircleEvent* event = nullptr;
    Parabola(sf::Vector2f focus, Parabola* a, Parabola* b): focus(focus), prev(a), next(b) {}
};

struct CircleEvent {
    sf::Vector2f center;
    float scanLinePos;
    Parabola* parabola;
    bool valid = true;
    CircleEvent(float scanLinePos, sf::Vector2f center, Parabola *parabola):
            scanLinePos(scanLinePos), center(center), parabola(parabola) {}
};

struct Comp {
    bool operator()(sf::Vector2f a, sf::Vector2f b) {
        return a.x == b.x ? a.y > b.y : a.x > b.x;
    }
    bool operator()(CircleEvent* a, CircleEvent* b) {
        return a->scanLinePos > b->scanLinePos;
    }
};

struct BoundingBox {
    float x1, x2, y1, y2;
    constexpr static const float margin = 50;
    BoundingBox(float x1, float x2, float y1, float y2): x1(x1), x2(x2), y1(y1), y2(y2) {};
    bool contains(sf::Vector2f point) {
        bool ans = true;
        ans &= (point.x >= x1);
        ans &= (point.x <= x2);
        ans &= (point.y >= y1);
        ans &= (point.y <= y2);
        return ans;
    }
};

auto parabolicFunc = [](float x, float y, float z, float l) -> float {
    return (x * x + y * y - l * l) / z;
};

std::priority_queue<sf::Vector2f, std::vector<sf::Vector2f>, Comp> siteEvents;
std::priority_queue<CircleEvent*, std::vector<CircleEvent*>, Comp> circleEvents;
BoundingBox boundingBox(0, appWidth, 0, appHeight);

bool scanLineShiftForCircleEvent(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c, float* shift, sf::Vector2f* center)
{
    if ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) > 0) {
        return false;
    }
    float A = b.x - a.x,  B = b.y - a.y,
           C = c.x - a.x,  D = c.y - a.y,
           E = A * (a.x + b.x) + B * (a.y + b.y),
           F = C * (a.x + c.x) + D * (a.y + c.y),
           G = 2 * (A * (c.y - b.y) - B * (c.x - b.x));
    if (G == 0) {
        return false;
    }
    center->x = (D * E - B * F) / G;
    center->y = (A * F - C * E) / G;
    *shift = center->x + std::sqrt((a.x - center->x) * (a.x - center->x) + (a.y - center->y) * (a.y - center->y));
    return true;
}

void createCircleEvent(Parabola* i, float scanLinePos)
{
    if (i->event != nullptr && i->event->scanLinePos != scanLinePos) {
        i->event->valid = false;
    }
    i->event = nullptr;
    if (i->prev == nullptr || i->next == nullptr) {
        return;
    }
    float newScanLinePos;
    sf::Vector2f center;
    if (scanLineShiftForCircleEvent(i->prev->focus, i->focus, i->next->focus, &newScanLinePos, &center) &&
        newScanLinePos > scanLinePos) {
        i->event = new CircleEvent(newScanLinePos, center, i);
        circleEvents.push(i->event);
    }
}

void processCircleEvent() {
    CircleEvent* event = circleEvents.top();
    circleEvents.pop();
    if (event->valid) {
        ParabolaSegment* segment = new ParabolaSegment(event->center);
        Parabola* parabola = event->parabola;
        if (parabola->prev) {
            parabola->prev->next = parabola->next;
            parabola->prev->seg2 = segment;
        }
        if (parabola->next) {
            parabola->next->prev = parabola->prev;
            parabola->next->seg1 = segment;
        }
        if (parabola->seg1) {
            parabola->seg1->finish(event->center);
        }
        if (parabola->seg2) {
            parabola->seg2->finish(event->center);
        }
        if (parabola->prev) {
            createCircleEvent(parabola->prev, event->scanLinePos);
        }
        if (parabola->next) {
            createCircleEvent(parabola->next, event->scanLinePos);
        }
    }
    delete event;
}

sf::Vector2f intersection(Parabola* parabola1, Parabola* parabola2, float scanLinePos)
{
    sf::Vector2f p1 = parabola1->focus, p2 = parabola2->focus;
    sf::Vector2f ans, point = p1;
    if (p1.x == p2.x) {
        ans.y = (p1.y + p2.y) / 2;
    } else if (p2.x == scanLinePos) {
        ans.y = p2.y;
    } else if (p1.x == scanLinePos) {
        ans.y = p1.y;
        point = p2;
    } else {
        float z1 = 2 * (p1.x - scanLinePos);
        float z2 = 2 * (p2.x - scanLinePos);
        float a = 1 / z1 - 1 / z2;
        float b = -2 * (p1.y / z1 - p2.y / z2);
        float c = parabolicFunc(p1.x, p1.y, z1, scanLinePos) -parabolicFunc(p2.x, p2.y, z2, scanLinePos);
        ans.y = ( -b - std::sqrt(b * b - 4 * a * c) ) / (2 * a);
    }
    ans.x = parabolicFunc(point.x, point.y - ans.y, 2 * (point.x - scanLinePos), scanLinePos);
    return ans;
}

bool newIntersection(sf::Vector2f focus, Parabola* parabola, sf::Vector2f* point)
{
    if (parabola->focus.x == focus.x) {
        return false;
    }
    float a, b;
    if (parabola->prev) {
        a = intersection(parabola->prev, parabola, focus.x).y;
    }
    if (parabola->next) {
        b = intersection(parabola, parabola->next, focus.x).y;
    }
    if ((parabola->prev == nullptr || a <= focus.y) && (parabola->next == nullptr || focus.y <= b)) {
        point->y = focus.y;
        point->x = parabolicFunc(parabola->focus.x, parabola->focus.y - point->y,
                                 2 * (parabola->focus.x - focus.x), focus.x);
        return true;
    }
    return false;
}

void processSiteEvents() {
    sf::Vector2f point = siteEvents.top();
    siteEvents.pop();
    if (root == nullptr) {
        root = new Parabola(point, nullptr, nullptr);
        return;
    }
    for (Parabola* i = root; i; i = i->next) {
        sf::Vector2f z, zz;
        if (newIntersection(point, i, &z)) {
            if (i->next != nullptr && !newIntersection(point, i->next, &zz)) {
                i->next->prev = new Parabola(i->focus, i, i->next);
                i->next = i->next->prev;
            } else  {
                i->next = new Parabola(i->focus, i, nullptr);
            }
            i->next->seg2 = i->seg2;
            i->next->prev = new Parabola(point, i, i->next);
            i->next = i->next->prev;
            i = i->next;
            i->prev->seg2 = i->seg1 = new ParabolaSegment(z);
            i->next->seg1 = i->seg2 = new ParabolaSegment(z);
            createCircleEvent(i, point.x);
            createCircleEvent(i->prev, point.x);
            createCircleEvent(i->next, point.x);
            return;
        }
    }
    Parabola* i;
    for (i = root; i->next != nullptr; i = i->next) {};
    i->next = new Parabola(point, i, nullptr);
    sf::Vector2f start;
    start.x = boundingBox.x2;
    start.y = (i->next->focus.y + i->focus.y) / 2;
    i->seg2 = i->next->seg1 = new ParabolaSegment(start);
};

void destroy() {
    if (root == nullptr) {
        return;
    }
    while (root->next != nullptr) {
        root = root->next;
        delete root->prev;
    }
    delete root;
    root = nullptr;
    while (!output.empty()) {
        delete output.back();
        output.pop_back();
    }
}

std::vector<std::pair<sf::Vector2f, sf::Vector2f>> solve(const std::vector<Wall>& points) {
    for (auto& p: points) {
        if (boundingBox.contains(p.v2f())) {
            siteEvents.push(p.v2f());
        }
    }
    boundingBox = BoundingBox(-BoundingBox::margin, appWidth + BoundingBox::margin,
                              -BoundingBox::margin, appHeight + BoundingBox::margin);
    while (!siteEvents.empty())
        if (!circleEvents.empty() && circleEvents.top()->scanLinePos <= siteEvents.top().x)
            processCircleEvent();
        else
            processSiteEvents();
    while (!circleEvents.empty())
        processCircleEvent();
    float scanLine = boundingBox.x1 + (boundingBox.x1-boundingBox.x2) + (boundingBox.y1-boundingBox.y2);
    for (Parabola* i = root; i->next != nullptr; i = i->next)
        if (i->seg2) {
            i->seg2->finish(intersection(i, i->next, scanLine * 2));
        }
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> ans;
    for (auto i = output.begin(); i != output.end(); i++) {
        ans.emplace_back((*i)->startPoint, (*i)->finishPoint);
    }
    destroy();
    return ans;
}
/////////////////////////////////////////////

class Scene {
private:
    std::vector<Wall> walls;
    std::vector<Segment> segments;
    bool isMouseInDelay = false;
public:
    Scene() = default;

    void process(sf::RenderWindow& window) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseInDelay) {
            walls.emplace_back(sf::Mouse::getPosition(window));
//            for (auto& wall: walls) {
//                segments.emplace_back(walls.back(), wall);
//            }
            segments.clear();
            for (auto p: solve(walls)) {
                segments.emplace_back(p.first, p.second);
            }
            isMouseInDelay = true;
        }
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isMouseInDelay = false;
        }
    }

    void draw(sf::RenderWindow& window) {
        for (auto& wall: walls) {
            wall.draw(window);
        }
        for (auto& seg: segments) {
            seg.draw(window);
        }
    }
};


int main() {
    sf::RenderWindow app(sf::VideoMode(appWidth, appHeight), "Chase");
    int frame_limit = 60;
    app.setFramerateLimit(frame_limit);
    app.setVerticalSyncEnabled(false);
    Scene scene;
    while (app.isOpen()) {
        sf::Event event;
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                app.close();
        }
        scene.process(app);
        app.clear(sf::Color(10, 10, 10));
        scene.draw(app);
        app.display();
    }
    return 0;
}
