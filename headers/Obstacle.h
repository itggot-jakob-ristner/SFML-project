#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "./Collider.h"
#include "./Animation.h"

// Object representing hitboxes that the player will stop at, such as walls
class Obstacle {
public:
    Obstacle(sf::Vector2f pos, sf::Vector2f size);
    ~Obstacle();

    Collider getCollider();
    sf::Vector2f getPos();
    sf::Vector2f getSize() {return body.getSize();};

private:
    sf::RectangleShape body;
    
};

// Links two maps together and the position where the player should be spawned
// in the new map. The name of the other map should be listed as a custom property
// in Tiled called "link" and the position under "linkPos".
// All of these are generated by the TileMap object responsible for the current map.
class CellDoor {
public:
    CellDoor(sf::Vector2f pos, sf::Vector2f size, std::string link, sf::Vector2f linkPos);
    ~CellDoor();

    Collider getCollider();
    std::string getLinkedMap();
    sf::Vector2f getLinkedPos();
private:
    sf::RectangleShape body;
    sf::Vector2f linkPos;
    std::string link;
};

// Represents an animated sprite which is part of the terrain such as a
// burning flame. It does not update its animation but shares it with all
// other sprites of the same type. This is to save space and avoid animations
// being offset by fractions of animation cycles.
class AnimatedTerrain: public sf::Sprite {
public:
    AnimatedTerrain(Animation &anim);
    ~AnimatedTerrain();

    void update();
    void draw(sf::RenderWindow &window);

private:
    Animation &anim;
};
