#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <list>
#include <string>
#include "baseClasses.cpp"
#include "menu.cpp"

int main() {
    sf::RenderWindow app(sf::VideoMode(app_width, app_height), "Red ball 0");
    int frame_limit = 60;
    app.setFramerateLimit(frame_limit);
    app.setVerticalSyncEnabled(false);

    load_textures();
    load_hotkeys();
    Menu menu;
    menu.loop(app);

    return EXIT_SUCCESS;
}
