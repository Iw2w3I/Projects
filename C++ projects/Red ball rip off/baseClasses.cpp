std::map<std::string, sf::Texture> global_textures;
std::map<std::string, sf::Keyboard::Key> global_hotkeys;

std::string key_to_str(sf::Keyboard::Key key) {
    if (key == sf::Keyboard::Right)
        return "->";
    if (key == sf::Keyboard::Left)
        return "<-";
    if (key == sf::Keyboard::Escape)
        return "esc";
    if (key == sf::Keyboard::Delete)
        return "delete";
    return {char('a' + key)};
}

std::vector<std::string> textures_for_rect = {
        "lava_texture.bmp",
        "lava_texture_line.bmp"};

void load_hotkeys() {
    global_hotkeys["save"] = sf::Keyboard::Z;
    global_hotkeys["load"] = sf::Keyboard::X;
    global_hotkeys["delete"] = sf::Keyboard::Delete;
    global_hotkeys["escape"] = sf::Keyboard::Escape;
    global_hotkeys["right"] = sf::Keyboard::D;
    global_hotkeys["left"] = sf::Keyboard::A;
    global_hotkeys["jump"] = sf::Keyboard::W;
    global_hotkeys["create_line"] = sf::Keyboard::L;
    global_hotkeys["create_rect"] = sf::Keyboard::R;
    global_hotkeys["create_checkpoint"] = sf::Keyboard::C;
    global_hotkeys["create_finish"] = sf::Keyboard::F;
    global_hotkeys["is_jumpable"] = sf::Keyboard::J;
    global_hotkeys["type"] = sf::Keyboard::T;
    global_hotkeys["pointless"] = sf::Keyboard::P;
    global_hotkeys["next_level"] = sf::Keyboard::N;
    global_hotkeys["is_scalable"] = sf::Keyboard::S;
    global_hotkeys["is_active"] = sf::Keyboard::I;
    global_hotkeys["prev"] = sf::Keyboard::Left;
    global_hotkeys["next"] = sf::Keyboard::Right;
}

void load_textures() {
    for (std::string& file: textures_for_rect) {
        sf::Image img;
        img.loadFromFile("../img/" + file);
        img.createMaskFromColor(sf::Color(255, 255, 255));
        sf::Texture t;
        t.loadFromImage(img);
        t.setRepeated(true);
        global_textures.emplace(file, std::move(t));
    }
}

class Sprite
{
    friend class Draw;
protected:
    std::string file;
    sf::Image img;
    sf::Sprite sprite;
    sf::Vector2f sprite_shift;
    bool is_sprite_valid = true, is_scalable = true;
public:
    Sprite(const std::string& File, sf::Vector2f sprite_point, sf::Vector2f Center): file(File) {
        if (file != "no") {
            if (!img.loadFromFile("../img/" + file))
                img.create(40, 40, sf::Color(255, 255, 255));
            else {
                img.createMaskFromColor(sf::Color(255, 255, 255));
            }
            if (!global_textures.contains(file)) {
                sf::Texture t;
                t.loadFromImage(img);
                t.setRepeated(true);
                global_textures.emplace(file, std::move(t));
            }
            sprite.setTexture(global_textures[file]);
            sprite.setPosition(sprite_point);
            sprite_shift = Center - sprite_point;
            sprite.setTextureRect(sf::Rect<int>({0, 0},
                                {int(std::abs(sprite_shift.x) * 2), int(std::abs(sprite_shift.y) * 2)}));
        } else
            is_sprite_valid = false;
    }
    const sf::Sprite& get_sprite() const {
        return sprite;
    }
    void set_pos(sf::Vector2f pos) {
        sprite.setPosition(pos - sprite_shift);
    }
    void set_is_scalable(bool b) {
        is_scalable = b;
        if (!b) {
            auto copy_scale = sprite.getScale();
            auto texture_rect = sprite.getTextureRect();
            sprite.setScale(sprite.getTextureRect().width / sprite.getLocalBounds().width,
                            sprite.getTextureRect().height / sprite.getLocalBounds().height);
            texture_rect.width = sprite.getLocalBounds().width * copy_scale.x;
            texture_rect.height = sprite.getLocalBounds().height * copy_scale.y;
            sprite.setTextureRect(texture_rect);
        }
    }
    void shift_pos(sf::Vector2f pos) {
        sprite.setPosition(sprite.getPosition() + pos);
    }
    void shift_size(sf::Vector2f pos) {
        sf::Rect<int> texture_rect = sprite.getTextureRect();
        sprite.setPosition(sprite.getPosition() - pos);
        if (!is_scalable) {
            texture_rect.width += 2 * pos.x;
            texture_rect.height += 2 * pos.y;
            if (texture_rect.width < 30) {
                texture_rect.width -= 2 * pos.x;
                sprite.setPosition(sprite.getPosition().x + pos.x, sprite.getPosition().y);
            }
            if (texture_rect.height < 30) {
                texture_rect.height -= 2 * pos.y;
                sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + pos.y);
            }
            sprite.setTextureRect(texture_rect);
        } else {
            sf::Vector2f scale = sprite.getScale(), copy = sprite.getScale();
            scale.x += 2 * pos.x / sprite.getTextureRect().width;
            scale.y += 2 * pos.y / sprite.getTextureRect().height;
            sprite.setScale(scale);
            if (sprite.getGlobalBounds().width < 30) {
                scale.x = copy.x;
                sprite.setPosition(sprite.getPosition().x + pos.x, sprite.getPosition().y);
            }
            if (sprite.getGlobalBounds().height < 30) {
                scale.y = copy.y;
                sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + pos.y);
            }
            sprite.setScale(scale);
        }
    }
    std::string& get_file() {
        return file;
    }
    void set_texture(const std::string& s) {
        file = s;
        sprite.setTexture(global_textures[file]);
    }
};

class Collision_Point;
class Collision_Line;

class Object {
protected:
    bool is_object_scaling = false, is_object_moving = false, draw_info = false;
    std::string tag;
    std::string location;
public:
    virtual const std::vector<sf::Text>& get_info() = 0;
    virtual bool get_is_sprite_valid() const = 0;
    virtual const sf::Shape& get_shape() const = 0;
    virtual const Sprite& get_sprite() const = 0;
    virtual bool is_point_inside(sf::Vector2f cursor_point) const = 0;
    virtual bool is_point_in_border(sf::Vector2f cursor_point) const = 0;
    virtual void shift_position(sf::Vector2f pos) = 0;
    virtual void shift_size(sf::Vector2f pos) = 0;
    virtual void move(sf::Vector2f prev_mouse_pos, sf::Vector2f mouse_pos) = 0;
    virtual void change() = 0;
    virtual int get_next_level() const = 0;
    virtual std::vector<Collision_Line> get_collision_lines() const = 0;
    virtual std::vector<Collision_Point> get_collision_points() const = 0;
    virtual sf::Vector2f get_center() = 0;
    bool get_is_object_scaling() const {
        return is_object_scaling;
    }
    void set_is_object_scaling(bool b) {
        is_object_scaling = b;
    }
    bool get_is_object_moving() const {
        return is_object_moving;
    }
    void set_is_object_moving(bool b) {
        is_object_moving = b;
    }
    bool get_do_draw_info() const {
        return is_object_moving;
    }
    void set_do_draw_info(bool b) {
        is_object_moving = b;
    }
    void set_tag(std::string new_tag) {
        tag = new_tag;
    }
    std::string get_tag() const {
        return tag;
    }
    void set_location(std::string new_location) {
        location = new_location;
    }
    std::string get_location() const {
        return location;
    }
};

class Text {
private:
    std::vector<sf::Text> text;
    sf::Text text_preparation;
    sf::Color color;
public:
    Text(sf::Font& font) {
        color = {255, 255, 255};
        text_preparation.setFont(font);
        text_preparation.setCharacterSize(18);
        text_preparation.setFillColor(sf::Color::White);
    }
    void str_to_text(const std::string& s, sf::Vector2f pos, int size = 18) {
        text_preparation.setString(s);
        text_preparation.setPosition(pos);
        text_preparation.setCharacterSize(size);
        text_preparation.setFillColor(color);
        text.push_back(text_preparation);
    }
    void clear() {
        text.clear();
    }
    void set_color(sf::Color Color) {
        color = Color;
    }
    const std::vector<sf::Text>& get_text() const {
        return text;
    }
};

class Button {
    friend class Draw;
private:
    sf::RectangleShape shape;
    sf::Text text;
    bool delay = false;
public:
    Button(sf::Vector2f Center, float Width, float Height, std::string&& Text, sf::Font& Font) {
        shape = sf::RectangleShape({Width, Height});
        shape.setPosition(Center.x - Width / 2, Center.y - Height / 2);
        shape.setFillColor(sf::Color::White);
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(3);
        text.setFont(Font);
        text.setString(Text);
        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(32);
        text.setPosition(Center.x - Width / 2, Center.y - 2 * Height / 3);
    }
    Button() = default;
    ~Button() = default;
    Button(const Button& b) = default;
    Button(Button&& b) = default;
    Button& operator=(const Button& b) = default;
    Button& operator=(Button&& b) = default;
    bool is_point_inside(sf::Vector2f point) const {
        return point.x < shape.getPosition().x + shape.getSize().x &&
               point.x > shape.getPosition().x &&
               point.y < shape.getPosition().y + shape.getSize().y &&
               point.y > shape.getPosition().y;

    }
    bool is_pressed(sf::RenderWindow &app) {
        return sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
               is_point_inside(sf::Vector2f(sf::Mouse::getPosition(app)));
    }
    bool is_pressed_with_delay(sf::RenderWindow &app) {
        bool ans = sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                    is_point_inside(sf::Vector2f(sf::Mouse::getPosition(app)));
        if (ans && !delay) {
            delay = true;
            return true;
        }
        if (!ans)
            delay = false;
        return false;
    }
    void set_string(std::string&& s) {
        text.setString(s);
    }
    void set_string(char s) {
        text.setString(s);
    }
    std::string get_string() const {
        return text.getString();
    }
};
