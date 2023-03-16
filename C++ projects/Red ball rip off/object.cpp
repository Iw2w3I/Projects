class Object_Point : public Sprite, public Collision_Point, public Object {
    friend class Draw;
private:
    sf::CircleShape shape;
    Text info;
public:
    Object_Point(const std::string& File, sf::Vector2f Point, bool Is_jumpable, sf::Font& font):
            Sprite(File, Point, Point), Collision_Point(Point, Is_jumpable), info(font) {
        if (!is_sprite_valid) {
            shape.setRadius(3);
            shape.setPosition(point);
            shape.setFillColor(sf::Color::White);
            shape.setOutlineColor(sf::Color::Black);
            shape.setOutlineThickness(1);
        }
    }
    bool get_is_sprite_valid() const override {
        return is_sprite_valid;
    }
    const sf::Shape& get_shape() const override {
        return shape;
    }
    const Sprite& get_sprite() const override {
        return *this;
    }
    sf::Vector2f get_center() override {
        return point;
    }
    std::vector<Collision_Line> get_collision_lines() const override {
        return {};
    }
    std::vector<Collision_Point> get_collision_points() const override {
        return {*this};
    }
    bool is_point_inside(sf::Vector2f cursor_point) const override {
        return distance_between_points(cursor_point, point) < 5;
    }
    bool is_point_in_border(sf::Vector2f cursor_point) const override {
        return false;
    }
    void shift_position(sf::Vector2f pos) override {
        shape.setPosition(shape.getPosition() + pos);
        Collision_Point::shift_pos(pos);
        Sprite::shift_pos(pos);
    }
    void shift_size(sf::Vector2f pos) override {}
    void move(sf::Vector2f prev_mouse_pos, sf::Vector2f mouse_pos) override {
        if (is_point_inside(prev_mouse_pos) || get_is_object_moving()) {
            set_is_object_moving(true);
            shift_position(mouse_pos - prev_mouse_pos);
        }
    }
    const std::vector<sf::Text>& get_info() override {
        return info.get_text();
    }
    void change() override {}
    int get_next_level() const override {
        return 0;
    }
};

class Object_Line : public Sprite, public Collision_Line, public Object {
    friend class Draw;
private:
    sf::RectangleShape shape;
    Text info;
    bool right_position = true, pointless = false;
    bool change_type_delay = false, change_is_jumpable_delay = false, change_pointless_delay = false;
    void update_line() {
        float prev_rotation = shape.getRotation();
        shape.setSize(sf::Vector2f(distance_between_points(point_a, point_b), 5));
        shape.setRotation(std::atan((point_a.y - point_b.y) / (point_a.x - point_b.x)) / acos(-1) * 180);
        if (std::abs(std::abs(shape.getRotation() - prev_rotation) - 180) < 30)
            right_position = !right_position;
        if (right_position)
            shape.setPosition(point_a);
        else
            shape.setPosition(point_b);
    }
    void info_update() {
        info.clear();
        float y = 0;
        info.str_to_text("object: line", {10, 10 + 20 * y++});
        info.str_to_text("point_a: " + std::to_string(point_a.x) +
                        " " + std::to_string(point_a.y), {10, 10 + 20 * y++});
        info.str_to_text("point_b: " + std::to_string(point_b.x) +
                        " " + std::to_string(point_b.y), {10, 10 + 20 * y++});
        info.str_to_text("angle: " + std::to_string(shape.getRotation()), {10, 10 + 20 * y++});
        info.str_to_text("type: " + std::string({type}), {10, 10 + 20 * y++});
        info.str_to_text("is_jumpable: " + std::to_string(is_jumpable), {10, 10 + 20 * y++});
        info.str_to_text("pointless: " + std::to_string(pointless), {10, 10 + 20 * y++});
    }
    void change_type() {
        if (key_press(global_hotkeys["type"], change_type_delay))
            type = (type == 'u' ? 'd' : 'u');
    }
    void change_is_jumpable() {
        if (key_press(global_hotkeys["is_jumpable"], change_is_jumpable_delay))
            is_jumpable = !is_jumpable;
    }
    void change_pointless() {
        if (key_press(global_hotkeys["pointless"], change_pointless_delay))
            pointless = !pointless;
    }
public:
    Object_Line(const std::string& File, sf::Vector2f Point_a, sf::Vector2f Point_b, char Type, bool Is_jumpable,
                bool Pointless, sf::Font& font):
            Sprite(File, Point_a, {(Point_a.x + Point_b.x) / 2, (Point_a.y + Point_b.y) / 2}),
            Collision_Line(Point_a, Point_b, Is_jumpable, Type), pointless(Pointless), info(font) {
        if (!Sprite::is_sprite_valid) {
            shape.setSize(sf::Vector2f(distance_between_points(point_a, point_b), 5));
            shape.setPosition(point_a);
            shape.setRotation(std::atan((point_a.y - point_b.y) / (point_a.x - point_b.x)) / acos(-1) * 180);
            shape.setFillColor(sf::Color::Black);
            shape.setOutlineColor(sf::Color::Black);
            shape.setOutlineThickness(1);
        }
    }
    bool get_is_sprite_valid() const override {
        return is_sprite_valid;
    }
    const sf::Shape& get_shape() const override {
        return shape;
    }
    const Sprite& get_sprite() const override {
        return *this;
    }
    sf::Vector2f get_point_a() {
        return point_a;
    }
    sf::Vector2f get_point_b() {
        return point_b;
    }
    sf::Vector2f get_center() override {
        return {(point_a.x + point_b.x) / 2, (point_a.y + point_b.y) / 2};
    }
    bool get_pointless() const {
        return pointless;
    }
    std::vector<Collision_Line> get_collision_lines() const override {
        return {*this};
    }
    std::vector<Collision_Point> get_collision_points() const override {
        if (pointless)
            return {};
        return {a, b};
    }
    bool is_point_inside(sf::Vector2f cursor_point) const override {
        return Collision_Line::is_point_inside(cursor_point);
    }
    bool is_point_in_border(sf::Vector2f cursor_point) const override {
        return distance_between_points(cursor_point, point_a) < 5 ||
                distance_between_points(cursor_point, point_b) < 5;
    }
    void shift_position(sf::Vector2f pos) override {
        shape.setPosition(shape.getPosition() + pos);
        Collision_Line::shift_pos(pos);
        Sprite::shift_pos(pos);
    }
    void shift_size(sf::Vector2f pos) override {
        Collision_Line::shift_size(pos);
        update_line();
        Sprite::shift_size(pos);
    }
    void move(sf::Vector2f prev_mouse_pos, sf::Vector2f mouse_pos) override {
        if (is_point_in_border(prev_mouse_pos) || get_is_object_scaling()) {
            if (std::abs(mouse_pos.x - prev_mouse_pos.x) < 30 &&
                std::abs(mouse_pos.y - prev_mouse_pos.y) < 30) {
                set_do_draw_info(true);
                set_is_object_scaling(true);
                shift_size(mouse_pos - prev_mouse_pos);
            }
        } else {
            if (is_point_inside(prev_mouse_pos) || get_is_object_moving()) {
                set_do_draw_info(true);
                set_is_object_moving(true);
                shift_position(mouse_pos - prev_mouse_pos);
            }
        }
    }
    const std::vector<sf::Text>& get_info() override {
        info_update();
        return info.get_text();
    }
    void change() override {
        change_type();
        change_is_jumpable();
        change_pointless();
    }
    int get_next_level() const override {
        return 0;
    }
};

class Object_Rectangle : public Sprite, public Collision_Rect, public Object {
    friend class Draw;
protected:
    sf::RectangleShape shape;
    Text info;
    bool change_is_jumpable_delay = false, change_texture_delay1 = false,
            change_texture_delay2 = false, change_is_scalable_delay = false;
    void info_update() {
        info.clear();
        float y = 0;
        info.str_to_text("object: rectangle", {10, 10 + 20 * y++});
        info.str_to_text("width: " + std::to_string(std::abs(left_l.get_point_a().x - right_l.get_point_a().x)),
                    {10, 10 + 20 * y++});
        info.str_to_text("height: " + std::to_string(std::abs(up_l.get_point_a().y - down_l.get_point_a().y)),
                    {10, 10 + 20 * y++});
        info.str_to_text("left up: " + std::to_string(left_l.get_point_a().x) +
                    " " + std::to_string(up_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("right up: " + std::to_string(right_l.get_point_a().x) +
                    " " + std::to_string(up_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("left down: " + std::to_string(left_l.get_point_a().x) +
                    " " + std::to_string(down_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("right down: " + std::to_string(right_l.get_point_a().x) +
                    " " + std::to_string(down_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("is_jumpable: " + std::to_string(up_l.get_is_jumpable()), {10, 10 + 20 * y++});
        info.str_to_text("is_scalable: " + std::to_string(is_scalable), {10, 10 + 20 * y++});
    }
    void change_is_jumpable() {
        if (key_press(global_hotkeys["is_jumpable"], change_is_jumpable_delay))
            up_l.set_is_jumpable(!up_l.get_is_jumpable());
    }
    void change_is_scalable() {
        if (key_press(global_hotkeys["is_scalable"], change_is_scalable_delay))
            set_is_scalable(!is_scalable);
    }
    void change_texture() {
        if (key_press(global_hotkeys["next"], change_texture_delay1)) {
            int i = std::find(textures_for_rect.begin(), textures_for_rect.end(), file) - textures_for_rect.begin();
            if (i - 1 >= 0) {
                set_texture(textures_for_rect[i - 1]);
            } else {
                set_texture(textures_for_rect.back());
            }
        }
        if (key_press(global_hotkeys["prev"], change_texture_delay2)) {
            int i = std::find(textures_for_rect.begin(), textures_for_rect.end(), file) - textures_for_rect.begin();
            if (i + 1 < textures_for_rect.size()) {
                set_texture(textures_for_rect[i + 1]);
            } else {
                set_texture(textures_for_rect[0]);
            }
        }
    }
public:
    Object_Rectangle(const std::string& File, sf::Vector2f Center, float Width, float Height, bool Is_jumpable,
                     sf::Font& font):
            Sprite(File, {Center.x - Width / 2, Center.y - Height / 2}, Center), info(font),
            Collision_Rect({Center.x - Width / 2, Center.y - Height / 2},
                           {Center.x + Width / 2, Center.y + Height / 2}, Is_jumpable) {
        if (!is_sprite_valid) {
            shape = sf::RectangleShape({Width, Height});
            shape.setPosition(Center.x - Width / 2, Center.y - Height / 2);
            shape.setFillColor(sf::Color::Black);
            shape.setOutlineColor(sf::Color::Black);
            shape.setOutlineThickness(3);
        }
    }
    bool get_is_sprite_valid() const override {
        return is_sprite_valid;
    }
    const sf::Shape& get_shape() const override {
        return shape;
    }
    const Sprite& get_sprite() const override {
        return *this;
    }
    sf::Vector2f get_center() override {
        return {(left_l.get_point_a().x + right_l.get_point_a().x) / 2,
                (up_l.get_point_a().y + down_l.get_point_a().y) / 2};
    }
    bool get_is_jumpable() const {
        return up_l.get_is_jumpable();
    }
    float get_width() {
        if (is_sprite_valid)
            return sprite.getTextureRect().width;
        return shape.getSize().x;
    }
    float get_height() {
        if (is_sprite_valid)
            return sprite.getTextureRect().height;
        return shape.getSize().y;
    }
    std::vector<Collision_Line> get_collision_lines() const override {
        return {up_l, down_l, left_l, right_l};
    }
    std::vector<Collision_Point> get_collision_points() const override {
        return {up_l.get_collision_point_a(), up_l.get_collision_point_b(),
                down_l.get_collision_point_a(), down_l.get_collision_point_b()};
    }
    bool is_point_inside(sf::Vector2f cursor_point) const override {
        return cursor_point.x < right_l.get_point_a().x && cursor_point.x > left_l.get_point_a().x
               && cursor_point.y < down_l.get_point_a().y && cursor_point.y > up_l.get_point_a().y;
    }
    bool is_point_in_border(sf::Vector2f cursor_point) const override {
        return up_l.is_point_inside(cursor_point) || down_l.is_point_inside(cursor_point) ||
               left_l.is_point_inside(cursor_point) || right_l.is_point_inside(cursor_point);
    }
    void shift_position(sf::Vector2f pos) override {
        shape.setPosition(shape.getPosition() + pos);
        Collision_Rect::shift_pos(pos);
        Sprite::shift_pos(pos);
    }
    void shift_size(sf::Vector2f pos) override {
        shape.setSize(shape.getSize() + pos + pos);
        shape.setPosition(shape.getPosition() - pos);
        if (shape.getSize().x < 30) {
            shape.setSize({shape.getSize().x - 2 * pos.x, shape.getSize().y});
            shape.setPosition(shape.getPosition().x + pos.x, shape.getPosition().y);
        }
        if (shape.getSize().y < 30) {
            shape.setSize({shape.getSize().x, shape.getSize().y - 2 * pos.y});
            shape.setPosition(shape.getPosition().x, shape.getPosition().y + pos.y);
        }
        Collision_Rect::shift_size(pos);
        Sprite::shift_size(pos);
    }
    void move(sf::Vector2f prev_mouse_pos, sf::Vector2f mouse_pos) override {
        if (is_point_in_border(prev_mouse_pos) || get_is_object_scaling()) {
            if (std::abs(mouse_pos.x - prev_mouse_pos.x) < 30 &&
                std::abs(mouse_pos.y - prev_mouse_pos.y) < 30) {
                set_do_draw_info(true);
                set_is_object_scaling(true);
                shift_size(mouse_pos - prev_mouse_pos);
            }
        } else {
            if (is_point_inside(prev_mouse_pos) || get_is_object_moving()) {
                set_do_draw_info(true);
                set_is_object_moving(true);
                shift_position(mouse_pos - prev_mouse_pos);
            }
        }
    }
    const std::vector<sf::Text>& get_info() override {
        info_update();
        return info.get_text();
    }
    void change() override {
        change_is_jumpable();
        change_texture();
        change_is_scalable();
    }
    int get_next_level() const override {
        return 0;
    }
};

class Checkpoint : public Object_Rectangle {
private:
    bool is_active = false, change_is_active_delay = false;
    void info_update() {
        info.clear();
        float y = 0;
        info.str_to_text("object: checkpoint", {10, 10 + 20 * y++});
        info.str_to_text("width: " + std::to_string(std::abs(left_l.get_point_a().x - right_l.get_point_a().x)),
                         {10, 10 + 20 * y++});
        info.str_to_text("height: " + std::to_string(std::abs(up_l.get_point_a().y - down_l.get_point_a().y)),
                         {10, 10 + 20 * y++});
        info.str_to_text("left up: " + std::to_string(left_l.get_point_a().x) +
                         " " + std::to_string(up_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("right up: " + std::to_string(right_l.get_point_a().x) +
                         " " + std::to_string(up_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("left down: " + std::to_string(left_l.get_point_a().x) +
                         " " + std::to_string(down_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("right down: " + std::to_string(right_l.get_point_a().x) +
                         " " + std::to_string(down_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("is_active: " + std::to_string(is_active), {10, 10 + 20 * y++});
    }
public:
    Checkpoint(const std::string& File, sf::Vector2f Center, float Width, float Height, sf::Font& font):
            Object_Rectangle(File, Center, Width, Height, false, font) {
        set_tag("checkpoint");
        if (!is_sprite_valid) {
            shape = sf::RectangleShape({Width, Height});
            shape.setPosition(Center.x - Width / 2, Center.y - Height / 2);
            shape.setFillColor(sf::Color::Green);
        }
    }
    void change() override {}
    const std::vector<sf::Text>& get_info() override {
        info_update();
        return info.get_text();
    }
};

class Finish : public Object_Rectangle {
private:
    int next_level;
    void info_update() {
        info.clear();
        float y = 0;
        info.str_to_text("object: finish", {10, 10 + 20 * y++});
        info.str_to_text("width: " + std::to_string(std::abs(left_l.get_point_a().x - right_l.get_point_a().x)),
                         {10, 10 + 20 * y++});
        info.str_to_text("height: " + std::to_string(std::abs(up_l.get_point_a().y - down_l.get_point_a().y)),
                         {10, 10 + 20 * y++});
        info.str_to_text("left up: " + std::to_string(left_l.get_point_a().x) +
                         " " + std::to_string(up_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("right up: " + std::to_string(right_l.get_point_a().x) +
                         " " + std::to_string(up_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("left down: " + std::to_string(left_l.get_point_a().x) +
                         " " + std::to_string(down_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("right down: " + std::to_string(right_l.get_point_a().x) +
                         " " + std::to_string(down_l.get_point_a().y), {10, 10 + 20 * y++});
        info.str_to_text("next_level: " + std::to_string(next_level), {10, 10 + 20 * y++});
    }
    void change_next_level() {
        if (key_press(global_hotkeys["next_level"], change_is_jumpable_delay))
            ++next_level;
    }
public:
    Finish(const std::string& File, sf::Vector2f Center, float Width, float Height, sf::Font& font, int Level):
            Object_Rectangle(File, Center, Width, Height, false, font), next_level(Level) {
        set_tag("finish");
        if (!is_sprite_valid) {
            shape = sf::RectangleShape({Width, Height});
            shape.setPosition(Center.x - Width / 2, Center.y - Height / 2);
            shape.setFillColor(sf::Color::Red);
        }
    }
    int get_next_level() const override {
        return next_level;
    }
    const std::vector<sf::Text>& get_info() override {
        info_update();
        return info.get_text();
    }
    void change() override {
        change_next_level();
    }
};