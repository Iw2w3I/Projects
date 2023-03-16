int app_width = 800, app_height = 600;

bool key_press(sf::Keyboard::Key button, bool& delay) {
    if (sf::Keyboard::isKeyPressed(button) && !delay) {
        delay = true;
        return true;
    }
    if (!sf::Keyboard::isKeyPressed(button))
        delay = false;
    return false;
}

void exit_press(sf::Keyboard::Key button, bool& exit, bool& delay) {
    if (sf::Keyboard::isKeyPressed(button))
        exit = true;
    else {
        exit = false;
        delay = false;
    }
}

#include "draw.cpp"
#include "game.cpp"
#include "redactor.cpp"

class LevelChoice {
    friend class Draw;
private:
    bool exit_delay = false, exit = false;
    sf::Font font;
    std::vector<Button> levels;
    Button back;
    sf::Texture background_text;
    sf::Sprite background;
    int force_next_level = 0;
public:
    LevelChoice(): back(Button({float(app_width / 12), float(app_height * 0.05)},
                               100, 30, "back", font)) {
        font.loadFromFile("../font/header2.ttf");
        background_text.loadFromFile("../img/menu_background.jpg");
        background.setTexture(background_text);
        background.setScale({app_width / background.getLocalBounds().width,
                                  app_height / background.getLocalBounds().height});
        for (int i = 1; i <= 10; ++i) {
            levels.push_back(Button({float(app_width * i / 11), float(app_height * 0.6)},
                                    30, 30, std::to_string(i), font));
        }
    }
    void loop(sf::RenderWindow &app) {
        while (app.isOpen() && !back.is_pressed(app) && !exit || exit_delay) {
            sf::Event event;
            while (app.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    app.close();
            }
            process(app);
            app.clear(sf::Color(10, 10, 10));
            Draw::level_choice(*this, app);
            app.display();
            if (back.is_pressed(app))
                break;
            exit_press(global_hotkeys["escape"], exit, exit_delay);
        }
    }
    void process(sf::RenderWindow& app) {
        if (force_next_level != 0) {
            Game new_game("../level" + std::to_string(force_next_level) + ".txt");
            force_next_level = new_game.loop(app);
            exit_delay = true;
        }
        if (levels[0].is_pressed(app)) {
            Game new_game("../level1.txt");
            force_next_level = new_game.loop(app);
            exit_delay = true;
        }
        if (levels[1].is_pressed(app)) {
            Game new_game("../level2.txt");
            force_next_level = new_game.loop(app);
            exit_delay = true;
        }
        if (levels[2].is_pressed(app)) {
            Game new_game("../level3.txt");
            force_next_level = new_game.loop(app);
            exit_delay = true;
        }
        if (force_next_level != 0)
            process(app);
    }
};

class Settings {
    friend class Draw;
private:
    bool exit_delay = false, exit = false, page_delay1 = false, page_delay2 = false;
    sf::Font font;
    std::vector<std::map<std::string, Button>> hotkeys;
    Button back, default_keys, next, prev;
    sf::Texture background_text;
    sf::Sprite background;
    sf::Texture frame_text;
    sf::Sprite frame;
    sf::Vector2f frame_shift;
    std::vector<Text> info;
    int page = 0, number_of_pages = 2;
    void info_update(int Page) {
        if (Page == 0) {
            info[0].clear();
            info[0].set_color({255, 255, 255});
            info[0].str_to_text("Hotkeys", {float(app_width / 2 - 80), 100}, 50);
            float x = 200, y = 188, k = 40, y_inc = 1;
            info[0].set_color({0, 0, 0});
            info[0].str_to_text("- movments (left, jump, right)", {x + 100, y - 5 + k * y_inc++}, 28);
            ++y_inc;
            info[0].str_to_text("- previous and next option", {x + 50, y - 5 + k * y_inc++}, 28);
            y_inc += 0.5;
            info[0].set_color({255, 255, 255});
            info[0].str_to_text("Hold the button and press a new key to change hotkeys",
                                {x - 80, y + k * y_inc++}, 22);
            info[0].set_color({0, 0, 0});
            y_inc += 0.5;
            info[0].str_to_text("- go back or exit", {x, y + k * y_inc++});
            info[0].str_to_text("- save your level in redactor", {x, y + k * y_inc++});
            info[0].str_to_text("- load your level in redactor", {x, y + k * y_inc++});
            info[0].str_to_text("- delete object in redactor", {x + 50, y + k * y_inc++});
        }
        if (Page == 1) {
            info[1].clear();
            info[1].set_color({255, 255, 255});
            info[1].str_to_text("Hotkeys", {float(app_width / 2 - 80), 100}, 50);
            float x = 200, y = 188, k = 40, y_inc = 0;
            info[1].set_color({0, 0, 0});
            info[1].str_to_text("- create a rectangular in redactor", {x, y + k * y_inc++});
            info[1].str_to_text("- create a line in redactor", {x, y + k * y_inc++});
            info[1].str_to_text("- create a checkpoint in redactor", {x, y + k * y_inc++});
            info[1].str_to_text("- create a finish mark in redactor", {x, y + k * y_inc++});
            info[1].str_to_text("- change object's parameter \'is_jumpable\' in redactor", {x, y + k * y_inc++});
            info[1].str_to_text("- change object's parameter \'type\' in redactor", {x, y + k * y_inc++});
            info[1].str_to_text("- change object's parameter \'pointless\' in redactor", {x, y + k * y_inc++});
            info[1].str_to_text("- change object's parameter \'next_level\' in redactor", {x, y + k * y_inc++});
            info[1].str_to_text("- change object's parameter \'is_scalable\' in redactor", {x, y + k * y_inc++});
        }
    }
    void change_key(sf::Keyboard::Key key, sf::RenderWindow& app) {
        for (auto& h: hotkeys[page])
            if (h.second.is_pressed_with_delay(app)) {
                h.second.set_string(key + 'a');
                global_hotkeys[h.first] = key;
            }
    }
    void default_hotkeys() {
        float x = 150, y = 200, k = 40, y_inc = 0;
        hotkeys[0]["up"] = Button({x + 50, y + k * y_inc++}, 30, 30, "w", font);
        hotkeys[0]["left"] = Button({x, y + k * y_inc}, 30, 30, "a", font);
        hotkeys[0]["right"] = Button({x + 100, y + k * y_inc++}, 30, 30, "d", font);
        ++y_inc;
        hotkeys[0]["prev"] = Button({x, y + k * y_inc}, 30, 30, "<-", font);
        hotkeys[0]["next"] = Button({x + 50, y + k * y_inc++}, 30, 30, "->", font);
        ++++y_inc;
        hotkeys[0]["escape"] = Button({x + 10, y + k * y_inc++}, 50, 30, "esc", font);
        hotkeys[0]["save"] = Button({x, y + k * y_inc++}, 30, 30, "z", font);
        hotkeys[0]["load"] = Button({x, y + k * y_inc++}, 30, 30, "x", font);
        hotkeys[0]["delete"] = Button({x + 35, y + k * y_inc++}, 100, 30, "delete", font);
        for (auto& h: hotkeys[0])
            h.second.set_string(key_to_str(global_hotkeys[h.first]));
        y_inc = 0;
        hotkeys[1]["create_rect"] = Button({x, y + k * y_inc++}, 30, 30, "r", font);
        hotkeys[1]["create_line"] = Button({x, y + k * y_inc++}, 30, 30, "l", font);
        hotkeys[1]["create_checkpoint"] = Button({x, y + k * y_inc++}, 30, 30, "c", font);
        hotkeys[1]["create_finish"] = Button({x, y + k * y_inc++}, 30, 30, "f", font);
        hotkeys[1]["is_jumpable"] = Button({x, y + k * y_inc++}, 30, 30, "j", font);
        hotkeys[1]["type"] = Button({x, y + k * y_inc++}, 30, 30, "t", font);
        hotkeys[1]["pointless"] = Button({x, y + k * y_inc++}, 30, 30, "p", font);
        hotkeys[1]["next_level"] = Button({x, y + k * y_inc++}, 30, 30, "n", font);
        hotkeys[1]["is_scalable"] = Button({x, y + k * y_inc++}, 30, 30, "s", font);
        for (auto& h: hotkeys[1])
            h.second.set_string(key_to_str(global_hotkeys[h.first]));
    }
public:
    Settings() {
        font.loadFromFile("../font/header2.ttf");
        background_text.loadFromFile("../img/menu_background.jpg");
        background.setTexture(background_text);
        background.setScale({app_width / background.getLocalBounds().width,
                             app_height / background.getLocalBounds().height});
        frame_shift = {50, 65};
        frame_text.loadFromFile("../img/woden_frame.jpg");
        frame.setTexture(frame_text);
        frame.setScale({(app_width - 2 * frame_shift.x) / frame.getLocalBounds().width,
                        app_height / frame.getLocalBounds().height});
        frame.setPosition(frame_shift);
        back = Button({float(app_width / 12), float(app_height * 0.05)},
                      100, 30, "back", font);
        default_keys = Button({float(app_width - 120), float(app_height * 0.05)},
                      200, 30, "default keys", font);
        prev = Button({float(290), float(130)},30, 30, "<", font);
        next = Button({float(520), float(130)},30, 30, ">", font);
        hotkeys = std::vector<std::map<std::string, Button>> (number_of_pages);
        default_hotkeys();
        info = std::vector<Text> (number_of_pages, font);
    }
    void loop(sf::RenderWindow& app) {
        while (app.isOpen() && !back.is_pressed(app) && !exit || exit_delay) {
            sf::Event event;
            while (app.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    app.close();
                if (event.type == sf::Event::KeyPressed)
                    change_key(event.key.code, app);
            }
            process(app);
            app.clear(sf::Color(10, 10, 10));
            Draw::settings(*this, app);
            app.display();
            if (back.is_pressed(app))
                break;
            exit_press(global_hotkeys["escape"], exit, exit_delay);
        }
    }
    void process(sf::RenderWindow& app) {
        if (default_keys.is_pressed_with_delay(app)) {
            load_hotkeys();
            default_hotkeys();
        }
        if (key_press(global_hotkeys["next"], page_delay1) || next.is_pressed_with_delay(app))
            page = (page + 1) % number_of_pages;
        if (key_press(global_hotkeys["prev"], page_delay2) || prev.is_pressed_with_delay(app))
            page = (number_of_pages + page - 1) % number_of_pages;
    }
    const std::vector<sf::Text>& get_info() {
        info_update(page);
        return info[page].get_text();
    }
};

class Menu {
    friend class Draw;
private:
    bool exit_delay = false, exit = false;
    sf::Font header_font;
    Button play, level_redactor, settings;
    sf::Texture menu_background_text;
    sf::Sprite menu_background;
public:
    Menu(): play({float(app_width * 0.8), float(app_height * 0.6)}, 200, 30,
                  "play", header_font),
                  level_redactor({float(app_width * 0.8),
                  float(app_height * 0.7)},200, 30, "build a level", header_font),
                  settings({float(app_width * 0.8),
                            float(app_height * 0.8)},200, 30, "settings", header_font){
        header_font.loadFromFile("../font/header2.ttf");
        menu_background_text.loadFromFile("../img/menu_background.jpg");
        menu_background.setTexture(menu_background_text);
        menu_background.setScale({app_width / menu_background.getLocalBounds().width,
                                  app_height / menu_background.getLocalBounds().height});
    }
    void loop(sf::RenderWindow& app) {
        while (app.isOpen() && !exit || exit_delay) {
            sf::Event event;
            while (app.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    app.close();
            }
            process(app);
            app.clear(sf::Color(10, 10, 10));
            Draw::menu(*this, app);
            app.display();
            exit_press(global_hotkeys["escape"], exit, exit_delay);
        }
    }
    void process(sf::RenderWindow &app) {
        if (play.is_pressed(app)) {
            LevelChoice choice;
            choice.loop(app);
            exit_delay = true;
        }
        if (level_redactor.is_pressed(app)) {
            Level_Redactor lr;
            lr.loop(app);
            exit_delay = true;
        }
        if (settings.is_pressed(app)) {
            Settings s;
            s.loop(app);
            exit_delay = true;
        }
    }
};