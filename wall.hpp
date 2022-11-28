#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

class Wall
{
public:
    Wall(sf::RenderWindow*, int, int);
    void draw();

private:
    sf::Sprite object;
    sf::Texture texture;
    sf::RenderWindow* window;
};

Wall::Wall(sf::RenderWindow* _window, int x, int y) : window(_window)
{
    texture.loadFromFile("PNG/Wall.png");
    object.setTexture(texture);
    object.setPosition((float)x, (float)y);
}

void Wall::draw()
{
    window->draw(object);
}