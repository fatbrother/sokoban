#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "wall.hpp"
#include "ground.hpp"
#include "player.hpp"
#include "crate.hpp"
#include "endPoint.hpp"

#define LIMIT_FPS 60

class Game
{
public:
    Game();
    Game(sf::RenderWindow *);
    void start();

private:
    static std::vector<int> dir;
    int type;
    bool isExit;
    bool isUpdate;
    std::pair<int, int> position;
    std::string nowFile;
    Player player;
    EndPoint endPoint;
    sf::View view;
    sf::Clock clock;
    sf::Sprite restart;
    sf::Texture restartTexture;
    sf::Sprite victory;
    sf::Texture victoryTexture;
    sf::Sprite exit;
    sf::Texture exitTexture;
    sf::RenderWindow *window;
    std::vector<Wall *> walls;
    std::vector<Crate *> crates;
    std::vector<Ground *> grounds;
    std::vector<std::string> mapFiles;
    std::vector<std::vector<char>> map;
    sf::RectangleShape curtain;

    void run();
    void loadMap();
    void setMap();
    void processEvents();
    void update();
    void render();
    bool isValid();
    bool isWin();
    void reset();
    void draw();
    std::string choseMap();
    void choseEvents(int &);
    void addMapImage(std::string);
    void initWindow();
    void transitionBegin();
    void transitionEnd();
};

std::vector<int> Game::dir = {0, 1, 0, -1, 0, 0};

Game::Game() {}

Game::Game(sf::RenderWindow *_window) : window(_window)
{
    isExit = false;
    type = STAND;
    restartTexture.loadFromFile("PNG/RestartBottom.png");
    restart.setTexture(restartTexture);
    victoryTexture.loadFromFile("PNG/Victory.png");
    victory.setTexture(victoryTexture);
    exitTexture.loadFromFile("PNG/Exit.png");
    exit.setTexture(exitTexture);
    initWindow();
}

void Game::initWindow()
{
    view.setSize(1800, 900);
    view.setCenter(900, 450);
    window->setView(view);
}

void Game::start()
{
    while (window->isOpen())
    {
        isExit = false;
        loadMap();
        nowFile = choseMap();
        if (isExit)
            break;
        setMap();
        run();
        initWindow();
    }
}

void Game::loadMap()
{
    std::ifstream map("map/maps.txt");

    while (!map.eof())
    {
        std::string fileName;
        map >> fileName;
        mapFiles.push_back(fileName);
    }
    map.close();
}

void Game::setMap()
{
    std::ifstream file("map/" + nowFile + ".txt");

    int y, x;
    file >> y >> x;

    map = std::vector<std::vector<char>>(x, std::vector<char>(y));
    view.setCenter(x * 32, y * 32);
    if (x / 2 > y)
        view.setSize(x * 64, x * 32);
    else
        view.setSize(y * 128, y * 64);
    window->setView(view);

    float dSize = (view.getSize().y / 900.f) * 0.5;
    restart.setPosition(dSize * 30 + view.getCenter().x - view.getSize().x / 2, dSize * 30 + view.getCenter().y - view.getSize().y / 2);
    restart.setScale(dSize, dSize);
    exit.setPosition(dSize * 30 + view.getCenter().x - view.getSize().x / 2, 40 * dSize + view.getCenter().y - view.getSize().y / 2 + restart.getGlobalBounds().height);
    exit.setScale(dSize, dSize);
    victory.setScale(dSize, dSize);
    float positionX = view.getCenter().x - victory.getGlobalBounds().width / 2, positionY = view.getCenter().y - victory.getGlobalBounds().height / 2;
    victory.setPosition(positionX, positionY);

    walls.clear();
    grounds.clear();
    crates.clear();
    endPoint.clear();
    for (int i = 0; i < y; i++)
        for (int j = 0; j < x; j++)
        {
            file >> map[j][i];
            switch (map[j][i])
            {
            case '/':
                walls.push_back(new Wall(window, j * 64, i * 64));
                break;
            case '-':
                grounds.push_back(new Ground(window, j * 64, i * 64));
                break;
            case '0':
                grounds.push_back(new Ground(window, j * 64, i * 64));
                player.set(window, j * 64, i * 64);
                map[j][i] = '-';
                break;
            case '1':
                grounds.push_back(new Ground(window, j * 64, i * 64));
                crates.push_back(new Crate(window, j * 64, i * 64));
                break;
            case '2':
                grounds.push_back(new Ground(window, j * 64, i * 64));
                endPoint.addPoint(window, j * 64, i * 64);
                map[j][i] = '-';
                break;
            default:
                break;
            }
        }
    file.close();

    transitionEnd();
}

void Game::run()
{
    isUpdate = true;
    while (window->isOpen() && !isExit)
    {
        processEvents();
        if (isUpdate)
        {
            update();
            render();
            if (isWin())
            {
                isUpdate = false;
                window->clear();
                draw();
                window->draw(victory);
                window->display();
            }
        }
    }
}

void Game::processEvents()
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
            case sf::Keyboard::Key::Up:
                player.sentOrder(UP);
                type = UP;
                break;
            case sf::Keyboard::Key::Down:
                player.sentOrder(DOWN);
                type = DOWN;
                break;
            case sf::Keyboard::Key::Left:
                player.sentOrder(LEFT);
                type = LEFT;
                break;
            case sf::Keyboard::Key::Right:
                player.sentOrder(RIGHT);
                type = RIGHT;
                break;
            case sf::Keyboard::Key::R:
                reset();
                break;
            case sf::Keyboard::Key::H:
                isExit = true;
                break;
            default:
                break;
            }
            break;
        case sf::Event::EventType::MouseButtonPressed:
            if (event.mouseButton.x >= 30 &&
                event.mouseButton.x <= 130 &&
                event.mouseButton.y >= 30 &&
                event.mouseButton.y <= 130 &&
                isUpdate)
                reset();
            else if (event.mouseButton.x >= 30 &&
                     event.mouseButton.x <= 130 &&
                     event.mouseButton.y >= 140 &&
                     event.mouseButton.y <= 240)
                isExit = true;
            break;
        default:
            break;
        }
    }
}

void Game::reset()
{
    transitionBegin();
    setMap();
}

void Game::update()
{
    if (isValid())
    {
        player.update(type);

        std::pair<int, int> current = player.getPosition();

        if (map[current.first][current.second] == '1')
        {
            for (auto &i : crates)
                if (i->getPosition() == current)
                    i->update(type);
            std::swap(map[current.first][current.second], map[current.first + dir[type]][current.second + dir[type + 1]]);
        }
    }
    type = STAND;
}

bool Game::isValid()
{
    if (type == STAND)
        return false;

    std::pair<int, int> position = player.getPosition();
    int x = position.first, y = position.second;

    if (map[x + dir[type]][y + dir[type + 1]] == '-')
        return true;
    if (map[x + dir[type]][y + dir[type + 1]] == '1')
    {
        if (map[x + 2 * dir[type]][y + 2 * dir[type + 1]] != '-')
            return false;
        else
            return true;
    }
    return false;
}

void Game::render()
{
    window->clear(sf::Color::Black);

    draw();

    window->display();
}

void Game::draw()
{
    for (auto &i : grounds)
        i->draw();
    for (auto &i : walls)
        i->draw();
    endPoint.draw();
    for (auto &i : crates)
        i->draw();
    player.draw();
    window->draw(restart);
    window->draw(exit);
}

bool Game::isWin()
{
    bool res = true;
    for (auto &i : crates)
        res &= endPoint.isShined(i->getPosition());
    return res;
}

std::string Game::choseMap()
{
    std::list<int> order;
    for (int i = 0; i < (int)mapFiles.size(); i++)
        order.push_back(i);

    sf::Sprite thumbnail;
    sf::Texture thumbnailTexture;
    int cnt = 0;
    while (window->isOpen())
    {
        window->clear();
        if (!thumbnailTexture.loadFromFile("PNG/" + mapFiles[order.front()] + ".png"))
        {
            addMapImage(mapFiles[order.front()]);
            thumbnailTexture.loadFromFile("PNG/" + mapFiles[order.front()] + ".png");
        }
        thumbnail.setTexture(thumbnailTexture, true);
        thumbnail.setScale(0.8, 0.8);
        thumbnail.setPosition(180, 0);
        exit.setTexture(exitTexture);
        exit.setScale(0.6, 0.6);
        exit.setPosition(40, 40);
        if (cnt == 0)
        {
            cnt++;
            curtain.setSize(view.getSize());
            sf::Vector2f site = {view.getCenter().x - view.getSize().x * 0.5f, view.getCenter().y - view.getSize().y * 0.5f};
            curtain.setPosition(site);
            curtain.setFillColor(sf::Color::Black);

            sf::Clock clock;
            while (curtain.getPosition().x < view.getCenter().x + view.getSize().x * 0.5f)
            {
                processEvents();
                curtain.move(1200 * clock.restart().asSeconds(), 0);
                window->clear();
                window->draw(thumbnail);
                window->draw(exit);
                window->draw(curtain);
                window->display();
            }
        }

        window->draw(thumbnail);
        window->draw(exit);
        window->display();

        int type = 3;
        choseEvents(type);
        if (type == -1)
        {
            order.push_front(order.back());
            order.pop_back();
            type = 3;
        }
        if (type == 1)
        {
            order.push_back(order.front());
            order.pop_front();
            type = 3;
        }
        if (type == 0)
            break;
        if (type == 2)
        {
            isExit = true;
            break;
        }
        if (type == 3)
            continue;
    }

    transitionBegin();

    return mapFiles[order.front()];
}

void Game::choseEvents(int &type)
{
    sf::Event event;
    bool isEvent = false;
    while (window->pollEvent(event))
    {
        isEvent = true;
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
            case sf::Keyboard::Key::Left:
                type = -1;
                break;
            case sf::Keyboard::Key::Right:
                type = 1;
                break;
            case sf::Keyboard::Key::Enter:
                type = 0;
                break;
            case sf::Keyboard::Key::H:
                type = 2;
                break;
            default:
                break;
            }
            break;
        case sf::Event::EventType::MouseButtonPressed:
            if (exit.getLocalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                type = 2;
            break;
        default:
            break;
        }
    }
    if (!isEvent)
        type = 3;
}

void Game::addMapImage(std::string fileName)
{
    nowFile = fileName;
    std::ifstream file("map/" + nowFile + ".txt");

    int y, x;
    file >> y >> x;

    map = std::vector<std::vector<char>>(x, std::vector<char>(y));
    view.setCenter(x * 32, y * 32);
    if (x / 2 > y)
        view.setSize(x * 64, x * 32);
    else
        view.setSize(y * 128, y * 64);
    window->setView(view);

    walls.clear();
    grounds.clear();
    crates.clear();
    endPoint.clear();
    for (int i = 0; i < y; i++)
        for (int j = 0; j < x; j++)
        {
            file >> map[j][i];
            switch (map[j][i])
            {
            case '/':
                walls.push_back(new Wall(window, j * 64, i * 64));
                break;
            case '-':
                grounds.push_back(new Ground(window, j * 64, i * 64));
                break;
            case '0':
                grounds.push_back(new Ground(window, j * 64, i * 64));
                player.set(window, j * 64, i * 64);
                map[j][i] = '-';
                break;
            case '1':
                grounds.push_back(new Ground(window, j * 64, i * 64));
                crates.push_back(new Crate(window, j * 64, i * 64));
                break;
            case '2':
                grounds.push_back(new Ground(window, j * 64, i * 64));
                endPoint.addPoint(window, j * 64, i * 64);
                map[j][i] = '-';
                break;
            default:
                break;
            }
        }
    file.close();

    window->clear();
    for (auto &i : grounds)
        i->draw();
    for (auto &i : walls)
        i->draw();
    endPoint.draw();
    for (auto &i : crates)
        i->draw();
    player.draw();


    sf::Texture capture;
    capture.create(window->getSize().x, window->getSize().y);
    capture.update(*window);
    capture.copyToImage().saveToFile("PNG/" + fileName + ".png");
    initWindow();
}

void Game::transitionBegin()
{
    curtain.setSize(view.getSize());
    sf::Vector2f site = {view.getCenter().x - view.getSize().x * 1.5f, view.getCenter().y - view.getSize().y * 0.5f};
    curtain.setPosition(site);
    curtain.setFillColor(sf::Color::Black);

    sf::Clock clock;
    while (curtain.getPosition().x < view.getCenter().x - view.getSize().x * 0.5f)
    {
        processEvents();
        curtain.move(1200 * clock.restart().asSeconds(), 0);
        window->draw(curtain);
        window->display();
    }
}

void Game::transitionEnd()
{
    curtain.setSize(view.getSize());
    sf::Vector2f site = {view.getCenter().x - view.getSize().x * 0.5f, view.getCenter().y - view.getSize().y * 0.5f};
    curtain.setPosition(site);
    curtain.setFillColor(sf::Color::Black);

    sf::Clock clock;
    while (curtain.getPosition().x < window->getPosition().x + view.getSize().x)
    {
        window->clear(sf::Color::Black);
        processEvents();
        curtain.move(1200 * clock.restart().asSeconds(), 0);
        draw();
        window->draw(curtain);
        window->display();
    }
}