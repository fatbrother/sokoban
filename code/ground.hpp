#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

class Ground
{
public:
    Ground(sf::RenderWindow*, int, int);
    void draw();

private:
    sf::RenderWindow *window;
    sf::Texture texture;
    sf::Sprite object;
};

Ground::Ground(sf::RenderWindow *_window, int x, int y) : window(_window)
{
    texture.loadFromFile("PNG/Ground.png");
    object.setTexture(texture);
    object.setPosition((float)x, (float)y);
}

void Ground::draw()
{
    window->draw(object);
}