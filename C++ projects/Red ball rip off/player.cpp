const float player_acceleration = 0.3, player_max_speed = 5, player_jump_speed = 10;
const float player_speed_loss_big = 0.3, player_speed_loss_small = 0.8;

class Player : public Sprite, public Collision_Circle {
    friend class Physics;
private:
    sf::Vector2f velocity = {0, 0}, center, respawn_point;
    bool is_in_collision = false;
    bool is_enough_room_to_jump(Object* object) {
        return !object->is_point_inside({center.x, center.y - radius - player_jump_speed / 2});
    }
public:
    Player(const sf::String& File, sf::Vector2f Center, float Radius) :
            Sprite(File, {Center.x - Radius, Center.y - Radius}, Center),
            Collision_Circle(Center, Radius), center(Center), respawn_point(Center) {}
    sf::Vector2f get_position() const {
        return center;
    }
    sf::Vector2f get_respawn_point() const {
        return respawn_point;
    }
    void set_position(sf::Vector2f pos) {
        center = pos;
        Collision_Circle::set_pos(pos);
        Sprite::set_pos(pos);
    }
    void move_right() {
        if (velocity.x < player_max_speed)
            velocity.x += player_acceleration;
    }
    void move_left() {
        if (velocity.x > -player_max_speed)
            velocity.x -= player_acceleration;
    }
    void jump(std::vector<Object*>& objects) {
        for (auto i_obj: objects)
            if (!is_enough_room_to_jump(i_obj))
                return;
        for (auto i_obj: objects) {
            std::vector<Collision_Line> lines = i_obj->get_collision_lines();
            if (lines.size() == 1) {
                if (collision(lines[0]) && lines[0].get_is_jumpable())
                    velocity.y = -player_jump_speed;
            } else {
                bool check_points = true;
                for (auto l: lines)
                    if (collision(l) && l.get_is_jumpable()) {
                        velocity.y = -player_jump_speed;
                        check_points = false;
                    }
                if (!check_points)
                    continue;
            }
            std::vector<Collision_Point> points = i_obj->get_collision_points();
            for (auto &p: points)
                if (collision(p) && p.get_is_jumpable()) {
                    velocity.x = player_jump_speed * (center.x - p.get_point().x) / radius;
                    velocity.y = player_jump_speed * (center.y - p.get_point().y) / radius;
                }
        }
    }
    void update() {
        center += velocity;
        Collision_Circle::center = center;
        sprite.setPosition(center - sprite_shift);
    }
    void shift_position(sf::Vector2f pos) {
        center += pos;
        Collision_Circle::shift_pos(pos);
        Sprite::shift_pos(pos);
    }
};
