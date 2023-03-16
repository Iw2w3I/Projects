class Physics {
public:
    int static check_collision_events(std::vector<Object*>& objects, Player& player, sf::Vector2f& screen) {
        player.is_in_collision = false;
        int result = 0;
        for (auto i_obj: objects) {
            if (i_obj->get_tag() == "finish") {
                if (player.collision(i_obj))
                    result = i_obj->get_next_level();
                continue;
            }
            if (i_obj->get_tag() == "checkpoint") {
                if (player.collision(i_obj) && !i_obj->get_is_object_moving() && !i_obj->get_is_object_scaling()) {
                    player.respawn_point = i_obj->get_center();
                    screen = {0, 0};
                }
                continue;
            }
           std::vector<Collision_Line> lines = i_obj->get_collision_lines();
            float radius = player.radius;
            sf::Vector2f& center = player.center;
            sf::Vector2f& velocity = player.velocity;
            if (lines.size() == 1) {
                if (player.collision(lines[0])) {  //slope
                    player.is_in_collision = true;
                    float force = distance_between_points(velocity, {0, 0}) * player_speed_loss_small;
                    sf::Vector2f point = center + velocity;
                    velocity = lines[0].reflect(point, center) - center;
                    float new_force = distance_between_points(velocity, {0, 0});
                    if (new_force != 0) {
                        velocity.x *= force / new_force;
                        velocity.y *= force / new_force;
                    }
                    if (lines[0].get_type() == 'd')
                        center.y = lines[0].get_coord_y(center.x) + radius / std::cos(lines[0].get_angle()) + 1;
                    else
                        center.y = lines[0].get_coord_y(center.x) - radius / std::cos(lines[0].get_angle()) - 1;
                    continue;
                }
            } else {
                if (player.collision(lines[0])) {  //'u'
                    player.is_in_collision = true;
                    center.y = lines[0].get_point_a().y - radius;
                    velocity.y = -player_speed_loss_big * std::abs(velocity.y);
                    if (velocity.y > -player_acceleration)
                        velocity.y = -player_acceleration;
                    continue;
                }
                if (player.collision(lines[1])) {  //'d'
                    player.is_in_collision = true;
                    center.y = lines[1].get_point_a().y + radius + 2;
                    velocity.y = player_speed_loss_big * std::abs(velocity.y);
                    continue;
                }
                if (player.collision(lines[2])) {  //'l'
                    player.is_in_collision = true;
                    center.x = lines[2].get_point_a().x - radius - 1;
                    velocity.x = -player_speed_loss_small * std::abs(velocity.x);
                    continue;
                }
                if (player.collision(lines[3])) {  //'r'
                    player.is_in_collision = true;
                    center.x = lines[3].get_point_a().x + radius + 1;
                    velocity.x = player_speed_loss_small * std::abs(velocity.x);
                    continue;
                }
            }
            std::vector<Collision_Point> points = i_obj->get_collision_points();
            for (auto &p: points)
                if (player.collision(p)) {
                    player.is_in_collision = true;
                    float dist = distance_between_points(p.get_point(), center);
                    center.x = (center.x * radius - p.get_point().x * (radius - dist)) / dist;
                    center.y = (center.y * radius - p.get_point().y * (radius - dist)) / dist;
                    float force = distance_between_points(velocity, {0, 0}) * player_speed_loss_big;
                    velocity.x = force * (center.x - p.get_point().x) / radius;
                    velocity.y = force * (center.y - p.get_point().y) / radius;
                }
        }
        return result;
    }
    void static fall(Player& player) {
        if (player.velocity.y < 3 * player_max_speed)
            player.velocity.y += player_acceleration;
    }
    void static friction(Player& player) {
        if (std::abs(player.velocity.x) > 0.05)
            if (player.velocity.x < player_max_speed + player_acceleration)
                player.velocity.x *= 0.95;
            else
                player.velocity.x *= 0.99;
        else
            player.velocity.x = 0;
    }
};