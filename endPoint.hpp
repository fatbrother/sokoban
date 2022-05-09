#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

class EndPoint
{
public:
    EndPoint();
    void addPoint(sf::RenderWindow*, int, int);
    void draw();
    void clear();
    bool isShined(std::pair<int, int>);

private:
    sf::Texture texture;
    sf::RenderWindow* window;
    std::set<std::pair<int, int>> site;
    std::vector<sf::Sprite> points;
};

EndPoint::EndPoint()
{
    texture.loadFromFile("PNG/EndPoint.png");
}

void EndPoint::addPoint(sf::RenderWindow* _window, int x, int y)
{
    window = _window;
    points.push_back(sf::Sprite(texture));
    points.back().setPosition((float)x + 16.f, (float)y + 16.f);
    site.insert({x / 64, y / 64});
}

void EndPoint::draw()
{
    for (auto &i : points)
        window->draw(i);
}

bool EndPoint::isShined(std::pair<int, int> crate)
{
    return site.count(crate);
}

void EndPoint::clear()
{
    site.clear();
    points.clear();
}