#include "collision.cpp"
#include "object.cpp"
#include "player.cpp"
#include "physics.cpp"

class Game {
    friend class Draw;
public:
    Game(const std::string& File): player("ball.bmp", {250, 250}, 50) {
        background_text.loadFromFile("../img/lava_background.jpg");
        background.setTexture(background_text);
        load(File);
        font.loadFromFile("../font/header2.ttf");
    }
    int loop(sf::RenderWindow& app) {
        while (app.isOpen() && !exit) {
            sf::Event event;
            while (app.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    app.close();
                if (event.type == sf::Event::KeyPressed)
                    if (sf::Keyboard::isKeyPressed(global_hotkeys["escape"]))
                        exit = true;
            }
            int next_level = process(app);
            if (next_level != 0) {
                exit = false;
                return next_level;
            }
            app.clear(sf::Color(30, 30, 30));
            Draw::game(*this, app);
            app.display();
        }
        exit = false;
        return 0;
    };
    int process(sf::RenderWindow& app) {
        sf::Vector2f old_player_position = player.get_position();
        if (sf::Keyboard::isKeyPressed(global_hotkeys["right"]))
            player.move_right();
        if (sf::Keyboard::isKeyPressed(global_hotkeys["left"]))
            player.move_left();
        Physics::fall(player);
        int next_level = Physics::check_collision_events(objects, player, screen);
        if (next_level != 0) {
            exit = true;
            return next_level;
        }
        if (sf::Keyboard::isKeyPressed(global_hotkeys["jump"]))
            player.jump(objects);
        Physics::friction(player);
        player.update();
        screen_scroll(old_player_position);
        return 0;
    }
private:
    bool exit = false;
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

    void load(const std::string& File) {
        std::ifstream fin;
        fin.open(File);
        if (fin.fail()) {
            std::cerr << "Unable to open file \"" << File << "\"" << std::endl;
            return;
        }
        std::string file;
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
        fin >> screen.x >> screen.y;
        background.setPosition(screen);
        sf::Vector2f old_position = player.get_position();
        player.set_position(Center);
        screen_scroll(old_position);
        while (!fin.eof()) {
            fin >> c;
            if (c != '/')
                break;
            fin >> file >> Center.x >> Center.y >> Width >> Height >> Is_jumpable;
            Center += player.get_position();
            Object_Rectangle new_object(file, Center, Width, Height, Is_jumpable, font);
            mem_object_rectangles.push_back(new_object);
            objects.push_back(&mem_object_rectangles.back());
            objects.back()->set_location("mem_object_rectangles "
                                         + std::to_string(mem_object_rectangles.size() - 1));
        }
        while (!fin.eof()) {
            fin >> c;
            if (c != '/')
                break;
            fin >> file >> Point_a.x >> Point_a.y >> Point_b.x >> Point_b.y >> Type >> Pointless;
            Point_a += player.get_position();
            Point_b += player.get_position();
            Object_Line new_object(file, Point_a, Point_b, Type, Type == 'u', Pointless, font);
            mem_object_lines.push_back(new_object);
            objects.push_back(&mem_object_lines.back());
            objects.back()->set_location("mem_object_lines "
                                         + std::to_string(mem_object_lines.size() - 1));
        }
        while (!fin.eof()) {
            fin >> c;
            if (c != '/')
                break;
            fin >> file >> Center.x >> Center.y >> Width >> Height;
            Center += player.get_position();
            Checkpoint new_object(file, Center, Width, Height, font);
            mem_checkpoints.push_back(new_object);
            objects.push_back(&mem_checkpoints.back());
            objects.back()->set_location("mem_checkpoints "
                                         + std::to_string(mem_checkpoints.size() - 1));
        }
        while (!fin.eof()) {
            fin >> c;
            if (c != '/')
                break;
            fin >> file >> Center.x >> Center.y >> Width >> Height >> Level;
            Center += player.get_position();
            Finish new_object(file, Center, Width, Height, font, Level);
            mem_finish.push_back(new_object);
            objects.push_back(&mem_finish.back());
            objects.back()->set_location("mem_checkpoints "
                                         + std::to_string(mem_finish.size() - 1));
        }
        fin.close();
    }
    void screen_scroll(sf::Vector2f old_position) {
        if (player.get_position().y < 150) {
            sf::Vector2f delta = {0, old_position.y - player.get_position().y};
            for (auto i_obj: objects) {
                if (!i_obj->get_is_object_moving() && !i_obj->get_is_object_scaling())
                    i_obj->shift_position(delta);
            }
            player.shift_position(delta);
            background.setPosition(background.getPosition() + delta * background_coef);
        }
        if (player.get_position().y > 500) {
            sf::Vector2f delta = {0, old_position.y - player.get_position().y};
            for (auto i_obj: objects) {
                if (!i_obj->get_is_object_moving() && !i_obj->get_is_object_scaling())
                    i_obj->shift_position(delta);
            }
            player.shift_position(delta);
            background.setPosition(background.getPosition() + delta * background_coef);
        }
        if (player.get_position().x < 200) {
            sf::Vector2f delta = {old_position.x - player.get_position().x, 0};
            for (auto i_obj: objects) {
                if (!i_obj->get_is_object_moving() && !i_obj->get_is_object_scaling())
                    i_obj->shift_position(delta);
            }
            player.shift_position(delta);
            background.setPosition(background.getPosition() + delta * background_coef);
        }
        if (player.get_position().x > 600) {
            sf::Vector2f delta = {old_position.x - player.get_position().x, 0};
            for (auto i_obj: objects) {
                if (!i_obj->get_is_object_moving() && !i_obj->get_is_object_scaling())
                    i_obj->shift_position(delta);
            }
            player.shift_position(delta);
            background.setPosition(background.getPosition() + delta * background_coef);
        }
    }
};