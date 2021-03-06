#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include <climits>

// A collection of nodes is the graph which will be traversed by the pathfinder.
// All of the default values are assigned according to what A* needs in order to
// function
struct Node {
    std::vector<Node*> neighbours;
    float startDistance = std::numeric_limits<float>::infinity(); // G Score
    float endDistance = std::numeric_limits<float>::infinity(); // H Score
    float prev = std::numeric_limits<float>::infinity();
    int x;
    int y;

    bool walkable = true;
    bool inRange = true;
    Node *cameFrom;
};

// An object which can find the shortest path between two nodes in a graph.
// Args:
// thinkInterval - how many milliseconds there should be between recalculations
class Pathfinder {
public:
    Pathfinder();
    Pathfinder(float thinkInterval);
    ~Pathfinder();
    void generateGraph(std::vector<std::vector<bool>> inpTiles, float resolutionMult);
    void setStartNode(int x, int y);
    void setEndNode(int x, int y);
    void generateGraphTexture(sf::RenderTexture &graphTexture, sf::Sprite graphSprite);
    void generatePathTexture(sf::RenderTexture &pathTexture, sf::Sprite &pathSprite);
    void update(float dt);
    std::vector<sf::Vector2f> getPath();

protected:
    void queuePush(Node *node);
    void reconstuctPath();
    Node *queuePop();
    float calcHValue(Node *node);
    bool isNodeClosed(Node *node);
    bool isNodeOpen(Node *node);
    void findPath();

    Node *startNode;
    Node *targetNode;
    std::vector<std::vector<Node>> allNodes;
    std::vector<Node*> closedNodes;
    std::vector<Node*> openNodes; // Priorityqueue - lowest f score first

    float vertexDistance;

    std::vector<Node*> path;

    // Timer stuff
    float currTime = 0;
    float thinkInterval;

    bool alreadyFound = false;
};

// Subclass of pathfinder which has some improvements specific to concurrent 
// pathfinding of the likes which is used in enemies that move around.
// Args:
// thinkInterval - how many milliseconds there should be between recalculations
class EnemyPathfinder : public Pathfinder {
public:
    EnemyPathfinder();
    EnemyPathfinder(float thinkInterval);
    ~EnemyPathfinder();

    void update(float dt, sf::Vector2f enemyPos, sf::Vector2f playerPos);
    void setAggroRange(float distance);
    void setMap(std::vector<std::vector<bool>> inpTiles);
    void increaseStep();
    void setCurrTime(float newTime);

    std::vector<sf::Vector2f> getPath();
    
protected:
    void updateRanges();
    void findPath();
    float getTotalCost();

    float aggroRange = std::numeric_limits<float>::infinity();
    sf::Vector2i position;
    int currentStep = 0;
};
