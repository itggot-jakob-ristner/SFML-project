# pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "./Player.h"
#include "./Settings.h"
#include "./Npc.h"
#include "./UiInterface.h"

class DevConsole {
public:
    DevConsole(Settings &settings, EnemyFactory &enemyFactory, UiGrid *grid, Player *player);
    ~DevConsole();

    bool open(sf::RenderWindow &window, Player &player);

private:
    void newParseCommand();
    void print(std::string message);

    void noclip();
    void setlevel();
    void setmovespeed();
    void setvisible();
    void setxlines();
    void setylines();
    void spawn();
    void setcolor();
    void tp();

    int searchCommands(std::string command);

    sf::Font fontFace;
    std::string currLine;
    std::vector<std::string> history;
    std::vector<std::string> words;
    // Must be in alphabetical order in order to work
    std::vector<std::string> commands = { "noclip",
    "setcolor",
    "setlevel",
    "setmovespeed",
    "setvisible",
    "setxlines",
    "setylines",
    "spawn",
    "tp" };
    std::vector<void (DevConsole::*)()> commandPointers;
    Settings &settings;
    EnemyFactory &enemyFactory;
    UiGrid *uiGrid;
    Player *player;

    unsigned short int index = 0;
};
