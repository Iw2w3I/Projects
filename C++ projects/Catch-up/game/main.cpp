#include <iostream>
#include <vector>
#include <cmath>
#include <deque>
#include <queue>
#include <set>
#include <SFML/Graphics.hpp>

int appWidth = 600, appHeight = 600;
float frame_limit = 60;

float dist(sf::Vector2f point1, sf::Vector2f point2) {
    return std::sqrt((point1.x - point2.x) * (point1.x - point2.x) +
                     (point1.y - point2.y) * (point1.y - point2.y));
}

float dist(sf::Vector2i point1, sf::Vector2i point2) {
    return std::sqrt((point1.x - point2.x) * (point1.x - point2.x) +
                     (point1.y - point2.y) * (point1.y - point2.y));
}

class Wall {
private:
    float x, y;
    sf::CircleShape shape;
    const float r = 3;
    void setShape(sf::Color color = sf::Color::White) {
        shape.setRadius(r);
        shape.setPointCount(8);
        shape.setPosition(x - r, y - r);
        shape.setFillColor(color);
//        setDebugShape();
    }
//    sf::CircleShape debugShape;
//    void setDebugShape() {
//        float r = 30;
//        debugShape.setRadius(r);
//        debugShape.setPointCount(16);
//        debugShape.setPosition(x - r, y - r);
//        debugShape.setOutlineColor(sf::Color::Green);
//        debugShape.setOutlineThickness(1);
//        debugShape.setFillColor(sf::Color::Transparent);
//    }
public:
    Wall() = default;
    Wall(float x, float y, sf::Color color = sf::Color::White): x(x), y(y) {
        setShape(color);
    }
    Wall(sf::Vector2i coords, sf::Color color = sf::Color::White): x(coords.x), y(coords.y) {
        setShape(color);
    }
    Wall(sf::Vector2f coords, sf::Color color = sf::Color::White): x(coords.x), y(coords.y) {
        setShape(color);
    }
    Wall& operator=(const Wall& w) {
        Wall copy;
        copy.x = w.x;
        copy.y = w.y;
        copy.setShape(w.shape.getFillColor());
        std::swap(*this, copy);
        return *this;
    };

    float getR() const {
        return r;
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
//        window.draw(debugShape);
    }
    sf::Vector2f v2f() const {
        return {x, y};
    }
    sf::CircleShape getShape() const {
        return shape;
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
    double angle() const {
        return a.x == b.x ? std::acos(-1) / 2 : std::atan((b.y - a.y) / (b.x - a.x));
    }
    sf::Vector2f shapePos() const {
        return (a.x < b.x || a.x == b.x && a.y < b.y) ? a : b;
    }
    void setShape(sf::Color color = sf::Color::White) {
        shape.setSize({length(), 1});
        shape.setRotation(angle() * 180 / std::acos(-1));
        shape.setPosition(shapePos());
        shape.setFillColor(color);
    }
public:
    Segment() = delete;
    Segment(sf::Vector2f point1, sf::Vector2f point2, sf::Color color = sf::Color::White): a(point1), b(point2) {
        setShape(color);
    }
    Segment(Wall& point1, Wall& point2, sf::Color color = sf::Color::White): a(point1.v2f()), b(point2.v2f()) {
        setShape(color);
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }
    std::vector<sf::Vector2f> partition(int k) const {
        std::vector<sf::Vector2f> ans = {a};
        for (int i = 0; i < k; ++i) {
            ans.emplace_back(ans.back().x + (b.x - a.x) / float(k), ans.back().y + (b.y - a.y) / float(k));
        }
        return ans;
    }
    float length() const {
        sf::Vector2f delta = a - b;
        return std::sqrt(delta.x * delta.x + delta.y * delta.y);
    }
    void shrink_to_lim(sf::Vector2f alim, sf::Vector2f blim, sf::Color color = sf::Color::White) {
        int sign = (b.x - a.x) / std::abs(b.x - a.x);
        if (a.x > b.x) { std::swap(a, b); }
        bool good_ax = (alim.x > a.x && alim.x < b.x || alim.x > b.x && alim.x < a.x);
        bool good_ay = (alim.y > a.y && alim.y < b.y || alim.y > b.y && alim.y < a.y);
        bool good_bx = (blim.x > a.x && blim.x < b.x || blim.x > b.x && blim.x < a.x);
        bool good_by = (blim.y > a.y && blim.y < b.y || blim.y > b.y && blim.y < a.y);
        float alim_coef = (good_ax ? (good_ay ? sign : (alim.x - a.x) / (b.x - a.x))
                                   : (good_ay ? (alim.y - a.y) / (b.y - a.y) : 0));
        float blim_coef = (good_bx ? (good_by ? sign : (blim.x - a.x) / (b.x - a.x))
                                   : (good_by ? (blim.y - a.y) / (b.y - a.y) : 1));
        if (good_ax && good_ay) {
            alim_coef *= std::min(sign * (alim.x - a.x) / (b.x - a.x), sign * (alim.y - a.y) / (b.y - a.y));
        }
        if (good_bx && good_by) {
            blim_coef *= std::min(sign * (blim.x - a.x) / (b.x - a.x), sign * (blim.y - a.y) / (b.y - a.y));
        }
        sf::Vector2f new_a = a + (b - a) * alim_coef;
        sf::Vector2f new_b = a + (b - a) * blim_coef;
        a = new_a;
        b = new_b;
        setShape(color);
    }
};

////////////////// Voronoi diagramm, Fortune's algorithm O(N log N), https://www.cs.hmc.edu/~mbrubeck/voronoi.html
class Voronoi {
private:
    struct ParabolaSegment;
    struct Parabola;
    struct CircleEvent;

    std::deque<ParabolaSegment*> output;
    Parabola* root = nullptr;

    struct ParabolaSegment {
        sf::Vector2f startPoint, finishPoint;
        bool complete = false;
        ParabolaSegment() = default;
        ParabolaSegment(sf::Vector2f point): startPoint(point), finishPoint(0,0) {};
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
        Parabola() = default;
        Parabola(sf::Vector2f focus, Parabola* a, Parabola* b): focus(focus), prev(a), next(b) {}
    };

    struct CircleEvent {
        sf::Vector2f center;
        float scanLinePos;
        Parabola* parabola;
        bool valid = true;
        CircleEvent() = default;
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
        BoundingBox() = default;
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

    float parabolicFunc(float x, float y, float z, float l) {
        return (x * x + y * y - l * l) / z;
    }

    std::priority_queue<sf::Vector2f, std::vector<sf::Vector2f>, Comp> siteEvents;
    std::priority_queue<CircleEvent*, std::vector<CircleEvent*>, Comp> circleEvents;
    BoundingBox boundingBox;

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
        *shift = center->x + dist(a, *center);
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
            output.push_back(&*segment);
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
                output.push_back(&*i->seg1);
                i->next->seg1 = i->seg2 = new ParabolaSegment(z);
                output.push_back(&*i->seg2);
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
        output.push_back(&*i->seg2);
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
public:
    Voronoi(): boundingBox(0, appWidth, 0, appHeight) {};

    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> solve(const std::vector<Wall>& points) {
        if (points.empty()) {
            return {};
        }
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
};



class SearchGraph {
private:
    float interval = 25;
    float botR;

    struct GraphHash {
        std::size_t operator()(const sf::Vector2i& p) const {
            return p.x + p.y * appWidth;
        }
    };
    std::vector<std::vector<std::pair<float, int>>> graph;
    std::vector<sf::Vector2i> vertexes;
    std::unordered_map<sf::Vector2i, int, GraphHash> indexes;


    void create_graph(std::vector<Wall>& walls) {
        int max_size = (appWidth * appHeight) / (interval * interval);
        graph.resize(max_size);
        vertexes.resize(max_size);
        int counter = 0;
        std::vector<sf::Vector2i> neighbors = {{1, 1},  {1, 0},  {1, -1},
                                               {0, 1},           {0, -1},
                                               {-1, 1}, {-1, 0}, {-1, -1}};
        for (int i = 1; i < appWidth / interval; ++i) {
            for (int j = 1; j < appHeight / interval; ++j) {
                sf::Vector2f point(i * interval, j * interval);
                bool toDelete = false;
                for (Wall& wall: walls) {
                    if (dist(point, wall.v2f()) < botR + wall.getR()) {
                        toDelete = true;
                        break;
                    }
                }
                if (toDelete) {
                    continue;
                }
                for (sf::Vector2i n: neighbors) {
                    sf::Vector2i neighbor((i + n.x) * interval, (j + n.y) * interval);
                    if (indexes.contains(neighbor)) {
                        graph[indexes[neighbor]].emplace_back(dist(point, sf::Vector2f(neighbor)), counter);
                        graph[counter].emplace_back(dist(point, sf::Vector2f(neighbor)), indexes[neighbor]);
                    }
                }
                vertexes[counter] = sf::Vector2i(point);
                indexes[sf::Vector2i(point)] = counter++;
            }
        }
        graph.resize(counter);
        vertexes.resize(counter);
    }

    bool legitPoint(sf::Vector2f point, std::vector<Wall>& walls) const {
        for (Wall& wall: walls) {
            if (dist(point, wall.v2f()) < botR + wall.getR()) { return false; }
        }
        return point.x >= interval && point.x <= appWidth - interval &&
               point.y >= interval && point.y <= appHeight - interval;
    }

    void addVertex(sf::Vector2f point, std::vector<Wall>& walls) { //adds a vertex and joins it to 4 closest vertexes in the grid
        if (!legitPoint(point, walls)) { return; }
        std::vector<sf::Vector2i> neighbors = {sf::Vector2i(int(point.x / interval) * interval,
                                                            int(point.y / interval) * interval)};
        neighbors.emplace_back(sf::Vector2i(neighbors[0].x, neighbors[0].y + interval));
        neighbors.emplace_back(sf::Vector2i(neighbors[0].x + interval, neighbors[0].y));
        neighbors.emplace_back(sf::Vector2i(neighbors[0].x + interval, neighbors[0].y + interval));
        std::vector<bool> connected(4);
        for (int i = 0; i < connected.size(); ++i) {
            connected[i] = indexes.contains(neighbors[i]);
        }
        if (!connected[0] && !connected[1] && !connected[2] && !connected[3]) { return; }
        indexes[sf::Vector2i(point)] = graph.size();
        vertexes.emplace_back(sf::Vector2i(point));
        graph.push_back({});
        for (int i = 0; i < connected.size(); ++i) {
            if (connected[i]) {
                graph.back().emplace_back(dist(sf::Vector2i(point), neighbors[i]), indexes[neighbors[i]]);
                graph[indexes[neighbors[i]]].emplace_back(dist(sf::Vector2i(point), neighbors[i]), graph.size() - 1);
            }
        }
    }

    bool uselessVoronoi(sf::Vector2f point, std::vector<Wall>& walls) const {
        float min_dist = dist(point, walls[0].v2f());
        int min_wall = 0;
        for (int i = 1; i < walls.size(); ++i) {
            float d = dist(point, walls[i].v2f());
            if (min_dist > d) {
                min_dist = d;
                min_wall = i;
            }
        }
        if (min_dist < botR + walls[min_wall].getR() ||
            min_dist > 1.6 * (botR + walls[min_wall].getR())) {
            return true;
        }
        return !(point.x >= interval && point.x <= appWidth - interval &&
                 point.y >= interval && point.y <= appHeight - interval);
    }

    void addVoronoi(std::vector<Wall>& walls, Voronoi& diagram) {
        int curr_index = graph.size();
        int start_index = curr_index;
        float interval_voronoi = interval / 4;
        for (auto p: diagram.solve(walls)) {
            Segment s(p.first, p.second);
            s.shrink_to_lim({0, 0}, sf::Vector2f(appWidth, appHeight));
            for (sf::Vector2f x: s.partition((s.length() + interval_voronoi - 1) / interval_voronoi)) {
                if (uselessVoronoi(x, walls)) {
                    continue;
                }
                float distance = dist(sf::Vector2i(x), vertexes[curr_index - 1]);
                if (distance > interval_voronoi) {
                    addVertex(x, walls);
                    ++curr_index;
                    continue;
                }
                addVertex(x, walls);
                if (graph.size() > curr_index) {
                    graph[curr_index].push_back({distance, curr_index - 1});
                    graph[curr_index - 1].push_back({distance, curr_index});
                    ++curr_index;
                }
            }
        }
        for (int i = start_index; i < graph.size(); ++i) {
            for (int j = i + 1; j < graph.size(); ++j) {
                float d = dist(vertexes[i], vertexes[j]);
                if (d < interval / 2 && legitPoint(sf::Vector2f((vertexes[i] + vertexes[j]) / 2), walls)) {
                    graph[i].push_back({d, j});
                    graph[j].push_back({d, i});
                }
            }
        }
    }
public:
    SearchGraph() = default;
    SearchGraph(std::vector<Wall>& walls, Voronoi& diagram, float botR): botR(botR * 0.92) {
        create_graph(walls);
        addVoronoi(walls, diagram);
    }

    void draw(sf::RenderWindow& window) const {
        for (auto p: indexes) {
            for (auto neighbor: graph[p.second]) {
                Segment(sf::Vector2f(p.first), sf::Vector2f(vertexes[neighbor.second]),
                        sf::Color(100, 100, 100)).draw(window);
            }
        }
    }

    std::vector<float> deykstraDist(int start, float speed) {
        std::vector<float> dist(graph.size(), std::numeric_limits<float>::max());
        std::vector<bool> used(graph.size(), false);
        dist[start] = 0;
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>,
                std::greater<>> q;
        q.push({0, start});
        while (!q.empty()) {
            int v = q.top().second;
            q.pop();
            if (used[v])
                continue;
            used[v] = true;
            for (std::pair<float, int> e: graph[v]) {
                if (dist[e.second] > dist[v] + e.first / speed) {
                    dist[e.second] = dist[v] + e.first / speed;
                    q.push({dist[e.second], e.second});
                }
            }
        }
        return dist;
    }

    int deykstraWay(int start, int finish, float speed) { //returns next vertex of closest wat to finnish
        std::vector<float> dist(graph.size(), std::numeric_limits<float>::max());
        std::vector<bool> used(graph.size(), false);
        std::vector<int> prev(graph.size());
        std::vector<int> way;
        dist[start] = 0;
        prev[start] = -1;
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>,
                std::greater<>> q;
        q.push({0, start});
        while (!q.empty()) {
            int v = q.top().second;
            q.pop();
            if (used[v])
                continue;
            used[v] = true;
            for (std::pair<float, int> e: graph[v]) {
                if (e.second == finish) {
                    if (v == start) {
                        return v;
                    }
                    while (prev[v] != start) {
                        v = prev[v];
                    }
                    return v;
                }
                if (dist[e.second] > dist[v] + e.first / speed) {
                    dist[e.second] = dist[v] + e.first / speed;
                    q.push({dist[e.second], e.second});
                    prev[e.second] = v;
                }
            }
        }
        return -1;
    }

    size_t size() const {
        return graph.size();
    }

    sf::Vector2i get_vertex(int i) const {
        return vertexes[i];
    }

    int get_vertex(sf::Vector2i point) const {
        return indexes.at(point);
    }

    std::vector<std::pair<float, int>> operator[](int i) const {
        return graph[i];
    }
};

class Player {
protected:
    float x, y;
    sf::CircleShape shape;
    float speed = 20;
    sf::Vector2f velocity;
    float r = 30;
    sf::Vector2f start_pos;
    void setShape(sf::Color color = sf::Color::Red) {
        shape.setRadius(r);
        shape.setPointCount(32);
        shape.setPosition(x - r, y - r);
        shape.setFillColor(color);
    }
public:
    Player() = default;
    Player(float x, float y, sf::Color color = sf::Color::Red, sf::Vector2f velocity = {0, 0}):
           x(x), y(y), velocity(velocity), start_pos(x, y) {
        setShape(color);
    }
    sf::Vector2f getPos() const {
        return {x, y};
    }
    float getR() const {
        return r;
    }
    float getSpeed() const {
        return speed;
    }
    sf::Vector2f getVelocity() const {
        return velocity;
    }
    bool intersect(Wall& wall) {
        return dist(getPos(), wall.v2f()) < getR() + wall.getR();
    }
    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }
    void setR(float R) {
        r = R;
        setShape(shape.getFillColor());
    }
    void setSpeed(float s) {
        speed = s;
    }
    void move(std::vector<char>& moveDirections) {
        float increase = speed / frame_limit;
        if (moveDirections.size() == 2) {
            increase /= std::sqrt(2);
        }
        for (char c: moveDirections) {
            if (c == 'D') {
                velocity.x += increase;
            }
            if (c == 'A') {
                velocity.x -= increase;
            }
            if (c == 'S') {
                velocity.y += increase;
            }
            if (c == 'W') {
                velocity.y -= increase;
            }
        }
        velocity.x *= 0.9;
        velocity.y *= 0.9;
        move(velocity);
    }
    void move(sf::Vector2f vec) {
        x += vec.x;
        y += vec.y;
        shape.move(vec);
    }
    void checkCollisions(std::vector<Wall>& walls) {
        for (auto& wall: walls) {
            if (intersect(wall)) {
                float dist1 = dist(getPos(), wall.v2f());
                float dist2 = getR() + wall.getR();
                sf::Vector2f direction = getPos() - wall.v2f();
                move(direction * (dist2 / dist1 - 1));
            }
        }
        if (x < getR()) {
            move({getR() - x, 0});
        }
        if (y < getR()) {
            move({0, getR() - y});
        }
        if (x > appWidth - getR()) {
            move({appWidth - getR() - x, 0});
        }
        if (y > appHeight - getR()) {
            move({0, appHeight - getR() - y});
        }
    }
    void restart() {
        move(start_pos - getPos());
    }
};

class Hunter : public Player {
    friend void hunt(Hunter& h1, Hunter& h2, const Player& target, std::vector<float>& minColor);
    friend void restrain(Hunter& h1, Hunter& h2, const Player& target, std::vector<float>& color);
    friend void drawHunting(Hunter& h1, Hunter& h2, sf::RenderWindow& window, std::vector<float>& color);
private:
    bool rush = false;
    sf::Vector2f currHunterPos, newHunterPoint;
    int currClosestHunter = -1, newClosestHunter = -1;
    std::deque<int> newHunterPointDeq;
    std::unordered_map<int, bool> newHunterPointMap;
    std::vector<char> autopilot;
    SearchGraph& graph;
public:
    Hunter(SearchGraph& graph, float x, float y,
           sf::Color color = sf::Color::Red, sf::Vector2f velocity = {0, 0}):
           graph(graph), Player(x, y, color, velocity) {};

    void stepTo(sf::Vector2f pos) {
        sf::Vector2f vec = pos - getPos();
        autopilot.clear();
        if (vec.x > 0) {
            autopilot.emplace_back('D');
        }
        if (vec.x < 0) {
            autopilot.emplace_back('A');
        }
        if (vec.y > 0) {
            autopilot.emplace_back('S');
        }
        if (vec.y < 0) {
            autopilot.emplace_back('W');
        }
    }

    int findVertex(const Player& target, int stepsAhead = 0) {
        float closestDist = std::numeric_limits<float>::max();
        int closest = -1;
        for (int i = 0; i < graph.size(); ++i) {
            sf::Vector2f targetPos = target.getPos();
            targetPos += target.getVelocity() * float(stepsAhead);
            float distance = dist(targetPos, sf::Vector2f(graph.get_vertex(i)));
            if (closestDist > distance) {
                closestDist = distance;
                closest = i;
            }
        }
        return closest;
    }

    sf::Vector2f attack(const Player& target, std::vector<float>& color) {
        int closestPray = findVertex(target, 0);
        int wayHunter = graph.deykstraWay(currClosestHunter, closestPray, speed);
        currHunterPos = sf::Vector2f(graph.get_vertex(currClosestHunter));
        color = std::vector<float> (graph.size(), 0);
        if (wayHunter != -1) {
            newClosestHunter = wayHunter;
            return sf::Vector2f(graph.get_vertex(wayHunter));
        }
        newClosestHunter = currClosestHunter;
        return sf::Vector2f(graph.get_vertex(currClosestHunter));
    }

    sf::Vector2f restrain(const Player& target, std::vector<float>& color) {
        int closestPray = findVertex(target, 1);
        currHunterPos = sf::Vector2f(graph.get_vertex(currClosestHunter));
        std::vector<float> distHunter = graph.deykstraDist(currClosestHunter, speed);
        std::vector<float> distPray = graph.deykstraDist(closestPray, target.getSpeed());
        int counterHunter = 0, counterPray = 0, bestHunter = currClosestHunter;
        for (int i = 0; i < graph.size(); ++i) {
            if (distHunter[i] - distPray[i] <= -2) {
                ++counterHunter;
            } else {
                ++counterPray;
            }
        }
        int copyCounterHunter = counterHunter;
        for (auto p: graph[currClosestHunter]) {
            distHunter = graph.deykstraDist(p.second, speed);
            int counterHunterAlt = 0;
            std::vector<float> colorAlt(distHunter.size());
            for (int i = 0; i < graph.size(); ++i) {
                colorAlt[i] = distHunter[i] - distPray[i];
                if (distHunter[i] - distPray[i] <= -2) {
                    ++counterHunterAlt;
                }
            }
            if (counterHunter < counterHunterAlt) {
                counterHunter = counterHunterAlt;
                bestHunter = p.second;
                color = colorAlt;
            }
        }
        if (counterHunter == copyCounterHunter) {
            return attack(target, color);
        }
        newClosestHunter = bestHunter;
        return sf::Vector2f(graph.get_vertex(bestHunter));
    }

    sf::Vector2f restrain(const Player& target, std::vector<float>& color, std::vector<float>& distHunter2) {
        int closestPray = findVertex(target, 1);
        currHunterPos = sf::Vector2f(graph.get_vertex(currClosestHunter));
        std::vector<float> distHunter = graph.deykstraDist(currClosestHunter, speed);
        std::vector<float> distPray = graph.deykstraDist(closestPray, target.getSpeed());
        int counterHunter = 0, counterPray = 0, bestHunter = currClosestHunter;
        for (int i = 0; i < graph.size(); ++i) {
            if (distHunter[i] - distPray[i] <= 0 || distHunter2[i] - distPray[i] <= 0) {
                ++counterHunter;
            } else {
                ++counterPray;
            }
        }
        int copyCounterHunter = counterHunter;
        for (auto p: graph[currClosestHunter]) {
            distHunter = graph.deykstraDist(p.second, speed);
            int counterHunterAlt = 0;
            std::vector<float> colorAlt(distHunter.size());
            for (int i = 0; i < graph.size(); ++i) {
                colorAlt[i] = std::min(distHunter[i] - distPray[i], distHunter2[i] - distPray[i]);
                if (distHunter[i] - distPray[i] <= 0 || distHunter2[i] - distPray[i] <= 0) {
                    ++counterHunterAlt;
                }
            }
            if (counterHunter < counterHunterAlt) {
                counterHunter = counterHunterAlt;
                bestHunter = p.second;
                color = colorAlt;
            }
        }
        if (counterHunter == copyCounterHunter) {
            return attack(target, color);
        }
        newClosestHunter = bestHunter;
        return sf::Vector2f(graph.get_vertex(bestHunter));
    }

    sf::Vector2f huntDesision(const Player& target, std::vector<float>& color) {
        return rush ? attack(target, color) : restrain(target, color);
    }

    void drawHunt(sf::RenderWindow& window, std::vector<float>& color) {
        for (int i = 0; i < graph.size(); ++i) {
            for (auto neighbor: graph[i]) {
                sf::Color c = sf::Color(120, 120, 120);
                if (!color.empty()) {
                    c = (color[i] < 0) ? sf::Color::Red : sf::Color::Green;
                    c = (color[i] == 0) ? sf::Color(40, 40, 40) : c;
                }
                Segment(sf::Vector2f(graph.get_vertex(i)), sf::Vector2f(graph.get_vertex(neighbor.second)),
                        c).draw(window);
            }
        }
        if (graph.size() > 0) {
            Wall(currHunterPos).draw(window);
        }
    }

    bool capture(Player& player) {
        return dist(getPos(), player.getPos()) < (player.getR() + getR()) * 0.9;
    }
    void restart() {
        move(start_pos - getPos());
        currClosestHunter = -1;
    }

    void update_position() {
        if (currClosestHunter == -1) {
            currClosestHunter = findVertex(*this);
            newClosestHunter = currClosestHunter;
        } else {
            if (dist(graph.get_vertex(currClosestHunter), sf::Vector2i(getPos())) >
                dist(graph.get_vertex(newClosestHunter), sf::Vector2i(getPos())))
                currClosestHunter = newClosestHunter;
        }
    }

    void rush_when_stack(const Player& target) {
        if (newHunterPointDeq.size() < 6) {
            int newPoint = newHunterPoint.x + newHunterPoint.y * graph.size();
            if (newHunterPointDeq.empty() || newHunterPointDeq.back() != newPoint) {
                newHunterPointDeq.emplace_back(newPoint);
            }
        } else {
            newHunterPointDeq.pop_front();
        }
        newHunterPointMap.clear();
        int count = 0;
        for (auto i: newHunterPointDeq) {
            count += !newHunterPointMap.contains(i);
            newHunterPointMap[i] = true;
        }
        rush = count < 5 || dist(getPos(), target.getPos()) < (getR() + target.getR()) * 1.5;
    }

    void hunt(const Player& target, std::vector<float>& minColor) {
        update_position();
        if (!autopilot.empty()) {
            move(autopilot);
        }
        newHunterPoint = huntDesision(target, minColor);
        rush_when_stack(target);
        stepTo(newHunterPoint);
    }
};

void restrain(Hunter& h1, Hunter& h2, const Player& target, std::vector<float>& color) {
    int closestPray = h1.findVertex(target, 1);
    h1.currHunterPos = sf::Vector2f(h1.graph.get_vertex(h1.currClosestHunter));
    h2.currHunterPos = sf::Vector2f(h2.graph.get_vertex(h2.currClosestHunter));
    std::vector<float> distPray = h1.graph.deykstraDist(closestPray, target.getSpeed());
    std::vector<float> distHunter1 = h1.graph.deykstraDist(h1.currClosestHunter, h1.speed);
    std::vector<float> distHunter2 = h2.graph.deykstraDist(h2.currClosestHunter, h2.speed);
    if (distHunter1[closestPray] > distHunter2[closestPray]) {
        restrain(h2, h1, target, color);
        return;
    }
    bool tooClose = (distHunter1[h2.currClosestHunter] < 5);

    int counterHunter = 0, counterPray = 0, bestHunter1 = h1.currClosestHunter, bestHunter2 = h2.currClosestHunter;
    for (int i = 0; i < h1.graph.size(); ++i) {
        if (distHunter1[i] - distPray[i] <= 0 || distHunter2[i] - distPray[i] <= 0) {
            ++counterHunter;
        } else {
            ++counterPray;
        }
    }
    int copyCounterHunter = counterHunter;
    float copyDist = distHunter1[h2.currClosestHunter];
    for (auto p: h1.graph[h1.currClosestHunter]) {
        distHunter1 = h1.graph.deykstraDist(p.second, h1.speed);
        if (distHunter1[h2.currClosestHunter] < copyDist && tooClose) {
            continue;
        }
        int counterHunterAlt = 0;
        std::vector<float> colorAlt(distHunter1.size());
        for (int i = 0; i < h1.graph.size(); ++i) {
            colorAlt[i] = std::min(distHunter1[i] - distPray[i], distHunter2[i] - distPray[i]);
            if (distHunter1[i] - distPray[i] <= 0 || distHunter2[i] - distPray[i] <= 0) {
                ++counterHunterAlt;
            }
        }
        if (counterHunter < counterHunterAlt) {
            counterHunter = counterHunterAlt;
            bestHunter1 = p.second;
            color = colorAlt;
        }
    }
    h1.newHunterPoint = (counterHunter == copyCounterHunter) ? h1.attack(target, color)
                                                             : sf::Vector2f(h1.graph.get_vertex(bestHunter1));
    h1.newClosestHunter = (counterHunter == copyCounterHunter) ? h1.newClosestHunter : bestHunter1;
    distHunter1 = h1.graph.deykstraDist(h1.newClosestHunter, h1.speed);
    copyCounterHunter = counterHunter;
    copyDist = distHunter2[h1.newClosestHunter];
    for (auto p: h2.graph[h2.currClosestHunter]) {
        distHunter2 = h2.graph.deykstraDist(p.second, h2.speed);
        if (distHunter2[h1.newClosestHunter] < copyDist && tooClose) {
            continue;
        }
        int counterHunterAlt = 0;
        std::vector<float> colorAlt(distHunter2.size());
        for (int i = 0; i < h2.graph.size(); ++i) {
            colorAlt[i] = std::min(distHunter1[i] - distPray[i], distHunter2[i] - distPray[i]);
            if (distHunter1[i] - distPray[i] <= 0 || distHunter2[i] - distPray[i] <= 0) {
                ++counterHunterAlt;
            }
        }
        if (counterHunter < counterHunterAlt) {
            counterHunter = counterHunterAlt;
            bestHunter2 = p.second;
            color = colorAlt;
        }
    }
    h2.newHunterPoint = (counterHunter == copyCounterHunter) ? h2.attack(target, color)
                                                              : sf::Vector2f(h2.graph.get_vertex(bestHunter2));
    h2.newClosestHunter = (counterHunter == copyCounterHunter) ? h2.newClosestHunter : bestHunter2;
}

void hunt(Hunter& h1, Hunter& h2, const Player& target, std::vector<float>& minColor) {
    h1.update_position();
    h2.update_position();
    if (!h1.autopilot.empty()) {
        h1.move(h1.autopilot);
    }
    if (!h2.autopilot.empty()) {
        h2.move(h2.autopilot);
    }
    h1.currHunterPos = sf::Vector2f(h1.graph.get_vertex(h1.currClosestHunter));
    h2.currHunterPos = sf::Vector2f(h2.graph.get_vertex(h2.currClosestHunter));
    std::vector<float> distHunter1 = h1.graph.deykstraDist(h1.currClosestHunter, h1.speed);
    std::vector<float> distHunter2 = h2.graph.deykstraDist(h2.currClosestHunter, h2.speed);
    if (h1.rush) {
        h1.newHunterPoint = h1.attack(target, minColor);
        h2.newHunterPoint = h2.restrain(target, minColor, distHunter1);
    } else {
        if (h2.rush) {
            h2.newHunterPoint = h2.attack(target, minColor);
            h1.newHunterPoint = h1.restrain(target, minColor, distHunter2);
        } else {
            restrain(h1, h2, target, minColor);
        }
    }
    h1.rush_when_stack(target);
    h2.rush_when_stack(target);
    h1.stepTo(h1.newHunterPoint);
    h2.stepTo(h2.newHunterPoint);
}

void drawHunting(Hunter& h1, Hunter& h2, sf::RenderWindow& window, std::vector<float>& color) {
    for (int i = 0; i < h1.graph.size(); ++i) {
        for (auto neighbor: h1.graph[i]) {
            sf::Color c = sf::Color(120, 120, 120);
            if (!color.empty()) {
                c = (color[i] < 0) ? sf::Color::Red : sf::Color::Green;
                c = (color[i] == 0) ? sf::Color(40, 40, 40) : c;
            }
            Segment(sf::Vector2f(h1.graph.get_vertex(i)), sf::Vector2f(h1.graph.get_vertex(neighbor.second)),
                    c).draw(window);
        }
    }
    Wall(h1.currHunterPos).draw(window);
    Wall(h2.currHunterPos).draw(window);
}

class Scene {
private:
    std::vector<Wall> walls;
    std::vector<Segment> segments;
    Player player;
    Hunter bot, bot2;
    SearchGraph graph;
    Voronoi diagram;
    std::vector<float> color;
    bool isMouseInDelay = false, cycle = false;
    bool drawVoronoi = false, drawGrid = false, drawHunt = false;
    bool drawVoronoiDelay = false, drawGridDelay = false, drawHuntDelay = false;

    sf::Clock clock;
    sf::Font font;
    sf::Text timeText;

public:
    Scene(): player(100, 100), bot(graph, 500, 500, sf::Color::Green), bot2(graph, 100, 500, sf::Color::Green) {
        bot.setSpeed(20);
        bot2.setSpeed(20);
        player.setSpeed(20);
//        player.setR(50);
//        bot.setR(50);
//        bot2.setR(50);
        graph = SearchGraph(walls, diagram, bot.getR());

        font.loadFromFile("../font/header2.ttf");
        timeText.setFont(font);
        timeText.setPosition(10, 10);
        timeText.setFillColor(sf::Color::White);
    };

    void process(sf::RenderWindow& window) {
        player.checkCollisions(walls);
        bot.checkCollisions(walls);
        bot2.checkCollisions(walls);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isMouseInDelay) {
            Wall newWall(sf::Mouse::getPosition(window));
            if (player.intersect(newWall) || bot.intersect(newWall) || bot2.intersect(newWall)) {
                return;
            }
            walls.emplace_back(newWall);
            graph = SearchGraph(walls, diagram, bot.getR());
//            for (auto& wall: walls) {
//                segments.emplace_back(walls.back(), wall);
//            }
            segments.clear();
            for (auto p: diagram.solve(walls)) {
                segments.emplace_back(p.first, p.second);
            }
            isMouseInDelay = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && !drawVoronoiDelay) {
            drawVoronoi = !drawVoronoi;
            drawVoronoiDelay = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && !drawGridDelay) {
            drawGrid = !drawGrid;
            drawGridDelay = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && !drawHuntDelay) {
            drawHunt = !drawHunt;
            drawHuntDelay = true;
        }
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isMouseInDelay = false;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
            drawVoronoiDelay = false;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
            drawGridDelay = false;
        }
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
            drawHuntDelay = false;
        }
        std::vector<char> moveDirections;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            moveDirections.emplace_back('D');
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            moveDirections.emplace_back('A');
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            moveDirections.emplace_back('W');
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            moveDirections.emplace_back('S');
        }
        player.move(moveDirections);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || cycle) {
            if (!cycle) {
                clock.restart();
            }
            cycle = true;
            timeText.setString(std::to_string(clock.getElapsedTime().asSeconds()));
            hunt(bot, bot2, player, color);
//            bot.hunt(player, color);
//            bot2.hunt(player, color);
        }
        if (bot.capture(player) || bot2.capture(player)) {
            player.restart();
            bot.restart();
            bot2.restart();
            cycle = false;
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(timeText);
        if (drawGrid) {
            graph.draw(window);
        }
        for (auto& wall: walls) {
            wall.draw(window);
        }
        if (drawVoronoi) {
            for (auto &seg: segments) {
                seg.draw(window);
            }
        }
        player.draw(window);
        bot.draw(window);
        bot2.draw(window);
        if (cycle && drawHunt) {
//            bot.drawHunt(window, color);
            drawHunting(bot, bot2, window, color);
        }
    }

    std::vector<Wall> get_walls() const {
        return walls;
    }

    void set_walls(std::vector<Wall> w) {
        walls = w;
        graph = SearchGraph(walls, diagram, bot.getR());
    }
};


int main() {
    sf::RenderWindow app(sf::VideoMode(appWidth, appHeight), "Chase");
    app.setFramerateLimit(frame_limit);
    app.setVerticalSyncEnabled(false);
    Scene scene;
    scene.set_walls({});
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
    for (Wall& w: scene.get_walls()) {
        std::cout << '{' << w.v2f().x << ", " << w.v2f().y << "}," << '\n';
    }
    return 0;
}
