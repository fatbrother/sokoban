#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

class Crate
{
public:
    Crate(sf::RenderWindow*, int, int);
    void draw();
    std::pair<int, int> getPosition();
    void update(int);

private:
    static std::vector<int> dir;
    std::pair<int, int> position;
    sf::RenderWindow* window;
    sf::Texture texture;
    sf::Sprite object;  
};

std::vector<int> Crate::dir = {0, 1, 0, -1, 0, 0};

Crate::Crate(sf::RenderWindow* _window, int x, int y) : window(_window)
{    
    object.setPosition((float)x, (float)y);
    texture.loadFromFile("PNG/Crate.png");
    object.setTexture(texture);
    position = {x / 64, y / 64};
}

void Crate::draw()
{
    window->draw(object);
}

void Crate::update(int type)
{
    position.first += dir[type];
    position.second += dir[type + 1];
    object.move(dir[type] * 64, dir[type + 1] * 64);
}


std::pair<int, int> Crate::getPosition()
{
    return position;
}