class Draw {
public:
    static void menu(auto& cls, sf::RenderWindow& window) {                     //cls type is Menu&
        window.draw(cls.menu_background);
        button(cls.play, window);
        button(cls.level_redactor, window);
        button(cls.settings, window);
    };
    static void level_choice(auto& cls, sf::RenderWindow& window) {             //cls type is LevelChoice&
        window.draw(cls.background);
        button(cls.back, window);
        for (auto& l: cls.levels)
            button(l, window);
    };
    static void settings(auto& cls, sf::RenderWindow& window) {                 //cls type is Settings&
        window.draw(cls.background);
        window.draw(cls.frame);
        button(cls.back, window);
        button(cls.default_keys, window);
        button(cls.next, window);
        button(cls.prev, window);
        for (auto& l: cls.hotkeys[cls.page])
            button(l.second, window);
        object_info(&cls, window);
    };
    static void button(auto& cls, sf::RenderWindow& window) {                   //cls type is Button&
        window.draw(cls.shape);
        window.draw(cls.text);
    }
    static void game(auto& cls, sf::RenderWindow& window) {                    //cls type is Game&
        window.draw(cls.background);
        for (auto i_obj: cls.objects) {
            object(i_obj, window);
            if (i_obj->get_do_draw_info())
                object_info(i_obj, window);
        }
        sprite(cls.player, window);
    }
    static void sprite(auto& cls, sf::RenderWindow& window) {                  //cls type is Sprite&
        window.draw(cls.sprite);
    }
    static void object(auto cls, sf::RenderWindow& window) {                   //cls type is Object*
        if (!cls->get_is_sprite_valid())
            window.draw(cls->get_shape());
        else
            sprite(cls->get_sprite(), window);
    }
    static void object_info(auto cls, sf::RenderWindow& window) {              //cls type is Object*
        for (auto i: cls->get_info())
            window.draw(i);
    }
};