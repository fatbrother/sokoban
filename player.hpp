#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

#define UP 2
#define LEFT 3
#define RIGHT 1
#define DOWN 0
#define STAND 4

class Player
{
public:
    Player();
    void update(int);
    void set(sf::RenderWindow*, int, int);
    void sentOrder(int);
    void draw();
    std::pair<int, int> getPosition();

private:
    static std::vector<int> dir;
    std::vector<sf::Texture> textures; 
    sf::Sprite object;
    sf::RenderWindow* window;
    std::pair<int, int> position;
};

std::vector<int> Player::dir = {0, 1, 0, -1, 0, 0};

Player::Player()
{
    textures.resize(4);
    textures[UP].loadFromFile("PNG/CharacterU.png");
    textures[DOWN].loadFromFile("PNG/CharacterD.png");
    textures[LEFT].loadFromFile("PNG/CharacterL.png");
    textures[RIGHT].loadFromFile("PNG/CharacterR.png");
}

void Player::set(sf::RenderWindow* _window, int x, int y)
{
    window = _window;
    object.setTexture(textures[DOWN]);
    object.setPosition((float)x + (64 - object.getTextureRect().width) / 2, (float)y + (64 - object.getTextureRect().height) / 2);
    position = {x / 64, y / 64};
}

void Player::draw()
{
    window->draw(object);
}

void Player::sentOrder(int type)
{
    object.setTexture(textures[type], true);
}

void Player::update(int type)
{
    position.first += dir[type];
    position.second += dir[type + 1];
    object.move(dir[type] * 64, dir[type + 1] * 64);
}

std::pair<int, int> Player::getPosition()
{
    return position;
}