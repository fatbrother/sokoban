#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

class Wall
{
public:
    Wall(sf::RenderWindow*, int, int);
    void draw();

private:
    static sf::Texture texture;
    sf::Sprite object;
    sf::RenderWindow* window;
};

Wall::texture.loadFromFile("PNG/Wall.png");

Wall::Wall(sf::RenderWindow* _window, int x, int y) : window(_window)
{
    object.setTexture(texture);
    object.setPosition((float)x, (float)y);
}

void Wall::draw()
{
    window->draw(object);
}