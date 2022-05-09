#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "game.hpp"

#define PLAY 1
#define EXIT 2

class Manu
{
public:
    Manu(sf::RenderWindow *);
    void start();

public:
    int mode;
    int dir;
    bool type;
    bool isStart;
    Game *game;
    sf::RenderWindow *window;
    sf::Sprite background;
    sf::Texture backgroundTexture;
    sf::Sprite title;
    sf::Texture titleTexture;
    sf::Sprite manuCharacter;
    std::vector<sf::Texture> manuCharacterTexture;
    sf::Sprite press;
    sf::Texture pressTexture;

    void processEvents();
    void update(float);
    void render();
    void transitionBegin();
    void transitionEnd();
};

Manu::Manu(sf::RenderWindow *_window) : window(_window)
{
    backgroundTexture.loadFromFile("PNG/Background.png");
    background.setTexture(backgroundTexture);
    background.setScale(1800.f / background.getTextureRect().width, 900.f / background.getTextureRect().height);
    titleTexture.loadFromFile("PNG/Title.png");
    title.setTexture(titleTexture);
    title.setScale(3, 3);
    title.setPosition(900 - title.getGlobalBounds().width / 2, 300 - title.getGlobalBounds().height / 2);
    manuCharacterTexture.resize(2);
    manuCharacterTexture[0].loadFromFile("PNG/ManuCharacterR.png");
    manuCharacterTexture[1].loadFromFile("PNG/ManuCharacterL.png");
    manuCharacter.setTexture(manuCharacterTexture[0]);
    manuCharacter.setScale(5, 5);
    manuCharacter.setPosition(-manuCharacter.getGlobalBounds().width, 900 - manuCharacter.getGlobalBounds().height);
    pressTexture.loadFromFile("PNG/Pressenter.png");
    press.setTexture(pressTexture);
    press.setScale(1.5, 1.5);
    press.setPosition(900 - press.getGlobalBounds().width / 2, 600 - manuCharacter.getGlobalBounds().height / 2);
    game = new Game(window);
    isStart = false;
    dir = 1;
    type = 0;
}

void Manu::start()
{
    sf::Clock clock;
    while (window->isOpen())
    {
        processEvents();
        if (isStart)
        {
            transitionBegin();
            game->start();
            isStart = false;
            transitionEnd();
        }
        update(clock.restart().asSeconds());
        render();
    }
}

void Manu::processEvents()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::EventType::Closed:
            window->close();
            break;
        case sf::Event::EventType::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::Key::Escape:
                window->close();
                break;
            case sf::Keyboard::Key::Enter:
                isStart = true;
                break;
            default:
                break;
            }
            break;
        case sf::Event::EventType::MouseButtonPressed:
            if (event.mouseButton.x >= 830 &&
                event.mouseButton.x <= 950 &&
                event.mouseButton.y >= 370 &&
                event.mouseButton.y <= 490)
                isStart = true;
            break;
        default:
            break;
        }
    }
}

void Manu::update(float time)
{
    manuCharacter.move(dir * 1800 * time, 0);
    if (manuCharacter.getGlobalBounds().left > manuCharacter.getGlobalBounds().width + 1800 ||
        manuCharacter.getGlobalBounds().left < -manuCharacter.getGlobalBounds().width)
    {
        dir = -dir;
        type = !type;
        manuCharacter.setTexture(manuCharacterTexture[type]);
        manuCharacter.move(dir * 1800 * time, 0);
    }
}

void Manu::render()
{
    window->clear();
    window->draw(background);
    window->draw(title);
    window->draw(manuCharacter);
    window->draw(press);
    window->display();
}

void Manu::transitionBegin()
{
    sf::RectangleShape curtain;
    sf::Vector2f size = {1800, 900};
    curtain.setSize(size);
    sf::Vector2f site = {-1800, 0};
    curtain.setPosition(site);
    curtain.setFillColor(sf::Color::Black);

    sf::Clock clock;
    while (curtain.getPosition().x < 0)
    {
        float time = clock.restart().asSeconds();
        curtain.move(1200 * time, 0);
        update(time);
        window->draw(background);
        window->draw(title);
        window->draw(manuCharacter);
        window->draw(press);
        window->draw(curtain);
        window->display();
    }
}

void Manu::transitionEnd()
{
    sf::RectangleShape curtain;
    sf::Vector2f size = {1800, 900};
    curtain.setSize(size);
    sf::Vector2f site = {0, 0};
    curtain.setPosition(site);
    curtain.setFillColor(sf::Color::Black);

    sf::Clock clock;
    while (curtain.getPosition().x < 1800)
    {
        float time = clock.restart().asSeconds();
        curtain.move(1200 * time, 0);
        update(time);
        window->draw(background);
        window->draw(title);
        window->draw(manuCharacter);
        window->draw(press);
        window->draw(curtain);
        window->display();
    }
}