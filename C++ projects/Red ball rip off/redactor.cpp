sf::Vector2f initial_screen = {0, -300};

class Level_Redactor {
    friend class Draw;
public:
    Level_Redactor(): player("ball.bmp", {250, 250}, 50) {
        Object_Rectangle ground("no", {400, 600}, 600, 30, true, font);
        mem_object_rectangles.push_back(ground);
        objects.push_back(&mem_object_rectangles.front());
        objects[0]->set_location("mem_object_rectangles " + std::to_string(object_id++));
        font.loadFromFile("../font/header2.ttf");
        background_text.loadFromFile("../img/lava_background.jpg");
        background.setTexture(background_text);
        screen = initial_screen;
        background.setPosition(screen);
    }
    void process(sf::RenderWindow& app) {
        sf::Vector2f old_player_position = player.get_position();
        create_object(app);
        move_objects(app);
        if (sf::Keyboard::isKeyPressed(global_hotkeys["right"]))
            player.move_right();
        if (sf::Keyboard::isKeyPressed(global_hotkeys["left"]))
            player.move_left();
        Physics::fall(player);
        Physics::check_collision_events(objects, player, screen);
        if (sf::Keyboard::isKeyPressed(global_hotkeys["jump"]))
            player.jump(objects);
        Physics::friction(player);
        player.update();
        screen_scroll(old_player_position);
        save();
        load();
    }
    void loop(sf::RenderWindow& app) {
        while (app.isOpen() && !exit || exit_delay) {
            sf::Event event;
            while (app.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    app.close();
            }
            process(app);
            app.clear(sf::Color(30,30,30));
            Draw::game(*this, app);
            app.display();
            exit_press(global_hotkeys["escape"], exit, exit_delay);
        }
    }
private:
    bool exit_delay = false, exit = false;
    std::vector<Object*> objects;
    std::list<Checkpoint> mem_checkpoints;
    std::list<Object_Rectangle> mem_object_rectangles;
    std::list<Object_Line> mem_object_lines;
    std::list<Finish> mem_finish;
    Player player;
    sf::Vector2f mouse_pos, prev_mouse_pos, screen;
    sf::Font font;
    bool key_delay = false, save_delay = false, load_delay = false;
    sf::Texture background_text;
    sf::Sprite background;
    float background_coef = 0.32;
    long long object_id = 0;

    void save() {
        if (key_press(global_hotkeys["save"], save_delay)) {
            std::ifstream fin;
            fin.open("../redactor.txt");
            if (fin.fail()) {
                std::cerr << "Unable to open file \"level.txt\"" << std::endl;
                return;
            }
            fin.close();
            std::ofstream fout;
            fout.open("../redactor.txt");
            fout << player.get_respawn_point().x << " " << player.get_respawn_point().y << '\n';
            fout << initial_screen.x << " " << initial_screen.y;
            for (auto& i: mem_object_rectangles) {
                fout << "\n/" << i.get_file() << " " << i.get_center().x - player.get_respawn_point().x - screen.x
                     << " " << i.get_center().y - player.get_respawn_point().y - screen.y << " " << i.get_width()
                     << " " << i.get_height() << " " << i.get_is_jumpable();
            }
            fout << "\n#";
            for (auto& i: mem_object_lines) {
                fout << "\n/" << i.get_file() << " " << i.get_point_a().x - player.get_respawn_point().x - screen.x
                     << " " << i.get_point_a().y - player.get_respawn_point().y - screen.y << " "
                     << i.get_point_b().x - player.get_respawn_point().x - screen.x << " "
                     << i.get_point_b().y - player.get_respawn_point().y - screen.y << " "
                     << i.get_type() << " " << i.get_pointless();
            }
            fout << "\n#";
            for (auto& i: mem_checkpoints) {
                fout << "\n/" << i.get_file() << " " << i.get_center().x - player.get_respawn_point().x - screen.x
                     << " " << i.get_center().y - player.get_respawn_point().y - screen.y << " " << i.get_width()
                     << " " << i.get_height();
            }
            fout << "\n#";
            for (auto& i: mem_finish) {
                fout << "\n/" << i.get_file() << " " << i.get_center().x - player.get_respawn_point().x - screen.x
                     << " " << i.get_center().y - player.get_respawn_point().y - screen.y << " " << i.get_width()
                     << " " << i.get_height() << " " << i.get_next_level();
            }
            fout << "\n#";
            fout.close();
        }
    }
    void load() {
        if (key_press(global_hotkeys["load"], load_delay)) {
            std::ifstream fin;
            fin.open("../redactor.txt");
            if (fin.fail()) {
                std::cerr << "Unable to open file \"level.txt\"" << std::endl;
                return;
            }
            std::string File;
            sf::Vector2f Center, Point_a, Point_b;
            float Width, Height;
            char c, Type;
            bool Pointless, Is_jumpable;
            int Level;
            mem_checkpoints.clear();
            mem_object_rectangles.clear();
            mem_object_lines.clear();
            mem_finish.clear();
            objects.clear();
            fin >> Center.x >> Center.y;
            fin >> initial_screen.x >> initial_screen.y;
            screen = initial_screen;
            background.setPosition(screen);
            sf::Vector2f old_position = player.get_position();
            player.set_position(Center);
            screen_scroll(old_position);
            while (!fin.eof()) {
                fin >> c;
                if (c != '/')
                    break;
                fin >> File >> Center.x >> Center.y >> Width >> Height >> Is_jumpable;
                Center += player.get_position();
                Object_Rectangle new_object(File, Center, Width, Height, Is_jumpable, font);
                mem_object_rectangles.push_back(new_object);
                objects.push_back(&mem_object_rectangles.back());
                objects.back()->set_location("mem_object_rectangles " + std::to_string(object_id++));
            }
            while (!fin.eof()) {
                fin >> c;
                if (c != '/')
                    break;
                fin >> File >> Point_a.x >> Point_a.y >> Point_b.x >> Point_b.y >> Type >> Pointless;
                Point_a += player.get_position();
                Point_b += player.get_position();
                Object_Line new_object(File, Point_a, Point_b, Type, Type == 'u', Pointless, font);
                mem_object_lines.push_back(new_object);
                objects.push_back(&mem_object_lines.back());
                objects.back()->set_location("mem_object_lines " + std::to_string(object_id++));
            }
            while (!fin.eof()) {
                fin >> c;
                if (c != '/')
                    break;
                fin >> File >> Center.x >> Center.y >> Width >> Height;
                Center += player.get_position();
                Checkpoint new_object(File, Center, Width, Height, font);
                mem_checkpoints.push_back(new_object);
                objects.push_back(&mem_checkpoints.back());
                objects.back()->set_location("mem_checkpoints " + std::to_string(object_id++));
            }
            while (!fin.eof()) {
                fin >> c;
                if (c != '/')
                    break;
                fin >> File >> Center.x >> Center.y >> Width >> Height >> Level;
                Center += player.get_position();
                Finish new_object(File, Center, Width, Height, font, Level);
                mem_finish.push_back(new_object);
                objects.push_back(&mem_finish.back());
                objects.back()->set_location("mem_finish " + std::to_string(object_id++));
            }
            fin.close();
        }
    }
    void move_objects(sf::RenderWindow& app) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            mouse_pos = sf::Vector2f(sf::Mouse::getPosition(app));
            int is_moving = -1;
            for (int i = 0; i < objects.size(); ++i)
                if (objects[i]->get_is_object_moving() || objects[i]->get_is_object_scaling())
                    is_moving = i;
            if (sf::Keyboard::isKeyPressed(global_hotkeys["delete"]) && is_moving != -1) {
                delete_object(app, is_moving);
                return;
            }
            if (is_moving != -1)
                for (auto i_obj: objects) {
                    if (i_obj != objects[is_moving]) {
                        i_obj->set_is_object_scaling(false);
                        i_obj->set_is_object_moving(false);
                        i_obj->set_do_draw_info(false);
                    }
                }
            for (auto i_obj: objects)
                if (is_moving == -1 || i_obj == objects[is_moving]) {
                    i_obj->move(prev_mouse_pos, mouse_pos);
                    i_obj->change();
                }
        } else {
            for (auto i_obj: objects) {
                i_obj->set_is_object_scaling(false);
                i_obj->set_is_object_moving(false);
                i_obj->set_do_draw_info(false);
            }
        }
        prev_mouse_pos = sf::Vector2f(sf::Mouse::getPosition(app));
    }
    void create_object(sf::RenderWindow& app) {
        create_checkpoint(app);
        create_object_rectangle(app);
        create_object_line(app);
        create_finish(app);
        if (!sf::Keyboard::isKeyPressed(global_hotkeys["create_line"])
                        && !sf::Keyboard::isKeyPressed(global_hotkeys["create_rect"])
                        && !sf::Keyboard::isKeyPressed(global_hotkeys["create_checkpoint"])
                        && !sf::Keyboard::isKeyPressed(global_hotkeys["create_finish"]))
            key_delay = false;
    }
    void create_checkpoint(sf::RenderWindow& app) {
        if (sf::Keyboard::isKeyPressed(global_hotkeys["create_checkpoint"]) && !key_delay) {
            key_delay = true;
            Checkpoint new_object("no", sf::Vector2f(sf::Mouse::getPosition(app)), 30, 100, font);
            mem_checkpoints.push_back(std::move(new_object));
            objects.push_back(&mem_checkpoints.back());
            objects.back()->set_location("mem_checkpoints " + std::to_string(object_id++));
        }
    }
    void create_object_rectangle(sf::RenderWindow& app) {
        if (sf::Keyboard::isKeyPressed(global_hotkeys["create_rect"]) && !key_delay) {
            key_delay = true;
            Object_Rectangle new_object("lava_texture.bmp", sf::Vector2f(sf::Mouse::getPosition(app)),
                                        100, 30, true, font);
            mem_object_rectangles.push_back(std::move(new_object));
            objects.push_back(&mem_object_rectangles.back());
            objects.back()->set_location("mem_object_rectangles " + std::to_string(object_id++));
        }
    }
    void create_object_line(sf::RenderWindow& app) {
        if (sf::Keyboard::isKeyPressed(global_hotkeys["create_line"]) && !key_delay) {
            key_delay = true;
            sf::Vector2f size(50, 30);
            Object_Line new_object("no", sf::Vector2f(sf::Mouse::getPosition(app)) - size,
                   sf::Vector2f(sf::Mouse::getPosition(app)) + size, 'u', true, false, font);
            mem_object_lines.push_back(std::move(new_object));
            objects.push_back(&mem_object_lines.back());
            objects.back()->set_location("mem_object_lines " + std::to_string(object_id++));
        }
    }
    void create_finish(sf::RenderWindow& app) {
        if (sf::Keyboard::isKeyPressed(global_hotkeys["create_finish"]) && !key_delay) {
            key_delay = true;
            Finish new_object("no", sf::Vector2f(sf::Mouse::getPosition(app)), 30, 100, font, 1);
            mem_finish.push_back(std::move(new_object));
            objects.push_back(&mem_finish.back());
            objects.back()->set_location("mem_finish " + std::to_string(object_id++));
        }
    }
    void delete_object(sf::RenderWindow& app, int i) {
        std::string loc = objects[i]->get_location(), list, index;
        int j = 0;
        while (loc[j] != ' ')
            list += loc[j++];
        if (list == "mem_checkpoints") {
            auto iter = mem_checkpoints.begin();
            while (iter->get_location() != loc)
                ++iter;
            mem_checkpoints.erase(iter);
        } else
            if (list == "mem_object_rectangles") {
                auto iter = mem_object_rectangles.begin();
                while (iter->get_location() != loc)
                    ++iter;
                mem_object_rectangles.erase(iter);
            } else
                if (list == "mem_object_lines") {
                    auto iter = mem_object_lines.begin();
                    while (iter->get_location() != loc)
                        ++iter;
                    mem_object_lines.erase(iter);
                } else
                    if (list == "mem_finish") {
                        auto iter = mem_finish.begin();
                        while (iter->get_location() != loc)
                            ++iter;
                        mem_finish.erase(iter);
                    }
        objects.erase(i + objects.begin());
    }
    void screen_scroll(sf::Vector2f old_position) {
        if (player.get_position().y < 150) {
            sf::Vector2f delta = {0, old_position.y - player.get_position().y};
            for (auto i_obj: objects) {
                if (!i_obj->get_is_object_moving() && !i_obj->get_is_object_scaling())
                    i_obj->shift_position(delta);
            }
            player.shift_position(delta);
            screen += delta;
        }
        if (player.get_position().y > 500) {
            sf::Vector2f delta = {0, old_position.y - player.get_position().y};
            for (auto i_obj: objects) {
                if (!i_obj->get_is_object_moving() && !i_obj->get_is_object_scaling())
                    i_obj->shift_position(delta);
            }
            player.shift_position(delta);
            screen += delta;
        }
        if (player.get_position().x < 200) {
            sf::Vector2f delta = {old_position.x - player.get_position().x, 0};
            for (auto i_obj: objects) {
                if (!i_obj->get_is_object_moving() && !i_obj->get_is_object_scaling())
                    i_obj->shift_position(delta);
            }
            player.shift_position(delta);
            screen += delta;
            background.setPosition(background.getPosition() + delta * background_coef);
        }
        if (player.get_position().x > 600) {
            sf::Vector2f delta = {old_position.x - player.get_position().x, 0};
            for (auto i_obj: objects) {
                if (!i_obj->get_is_object_moving() && !i_obj->get_is_object_scaling())
                    i_obj->shift_position(delta);
            }
            player.shift_position(delta);
            screen += delta;
            background.setPosition(background.getPosition() + delta * background_coef);
        }
    }
};