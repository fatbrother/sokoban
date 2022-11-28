#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "manu.hpp"

int main()
{
    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(1800, 900), "SOKOBAN--CreateByXUAN");
    Manu manu(window);
    manu.start();
    window->close();
    return 0;
}