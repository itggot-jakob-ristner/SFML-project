#include "../headers/Collider.h"
#include "../headers/Utils.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

// Abstract
CollisionBox::CollisionBox() {

}

// Abstract
CollisionBox::~CollisionBox() {

}

// Returns whether this collider and another is intersecting each other, this 
// does not however resolve the collision.
// Args:
// other - pointer to the other CollisionBox
bool CollisionBox::isColliding(CollisionBox *other) {
    sf::Vector2f thisPosition = getPosition();
    sf::Vector2f otherPosition = (*other).getPosition();
    sf::Vector2f thisHalfSize = getHalfSize();
    sf::Vector2f otherHalfSize = (*other).getHalfSize();

    float intersectX = std::abs(otherPosition.x - thisPosition.x) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = std::abs(otherPosition.y - thisPosition.y) - (otherHalfSize.y + thisHalfSize.y);

    return (intersectX < 0.0f && intersectY < 0.0f);

}

// Returns whether this collider and another intersect each other and 
// pushes them out of each other. The amount the other collider moves 
// is dictated by "push" and the amount this colliders body moves according
// to 1 - push.
// Args:
// other     - the other Collider checking collision with
// direction - reference to a vector which can be passed on to an objects
//             onCollision method if special behaviour occurs on collision
// push      - scale factor for how much the other collider should move, this 
//             collider moves according to 1 - push
bool CollisionBox::checkCollision(CollisionBox *other, sf::Vector2f& direction, float push) {
    sf::Vector2f otherPosition = (*other).getPosition();
    sf::Vector2f otherHalfSize = (*other).getHalfSize();
    sf::Vector2f thisPosition = getPosition();
    sf::Vector2f thisHalfSize = getHalfSize();

    float deltaX = otherPosition.x - thisPosition.x;
    float deltaY = otherPosition.y - thisPosition.y;

    float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
    float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

    if (intersectX < 0.0f && intersectY < 0.0f) {
        push = std::min(std::max(push, 0.0f), 1.0f);

        if (intersectX > intersectY) {
            if (deltaX > 0.0f) {
                move(intersectX * (1.0f - push), 0.0f);
                (*other).move(-intersectX  * push, 0.0f);

                direction.x = 1.0f;
                direction.y = 0.0f;
            } else {
                move(-intersectX * (1.0f - push), 0.0f);
                (*other).move(intersectX  * push, 0.0f);

                direction.x = -1.0f;
                direction.y = 0.0f;
            }
        } else {
            if (deltaY < 0.0f) {
                move(0.0f, -intersectY * (1.0f - push));
                (*other).move(0.0f, intersectY * push);

                direction.x = 0.0f;
                direction.y = 1.0f;
            } else {
                move(0.0f, intersectY * (1.0f - push));
                (*other).move(0.0f, -intersectY * push);

                direction.x = 0.0f;
                direction.y = -1.0f;
            }
        }
        return true;
    }
    return false;
}

Collider::Collider(sf::RectangleShape &body, sf::Vector2f velocity) :
    body(body){
    this->velocity = velocity;
}

Collider::~Collider() {

}

void Collider::move(float dx, float dy) {
    body.move(dx, dy);
}

sf::Vector2f Collider::getPosition() {
    return body.getPosition();
}

sf::Vector2f Collider::getHalfSize() {
    return body.getSize() / 2.0f;
}

SpriteCollider::SpriteCollider(sf::Sprite &sprite, sf::Vector2f velocity):
    body(sprite) {
        this->velocity = velocity;
}

SpriteCollider::~SpriteCollider() {

}

void SpriteCollider::move(float dx, float dy) {
    body.move(dx, dy);
}

sf::Vector2f SpriteCollider::getPosition() {
    return body.getPosition();
}

sf::Vector2f SpriteCollider::getHalfSize() {
    sf::IntRect boundRect = body.getTextureRect();

    return sf::Vector2f(boundRect.width, boundRect.height) / 2.0f;
}

CollisionCircle::CollisionCircle(sf::Vector2f epicenter, float radius) {
    this->epicenter = epicenter;
    this->radius = radius;

}

CollisionCircle::~CollisionCircle() {

}

bool CollisionCircle::isColliding(CollisionCircle *other) {
    if (getDistance(getEpicenter(), (*other).getEpicenter()) 
        < (getRadius() + (*other).getRadius())) {
        return true;
    }
    return false;
}

bool CollisionCircle::isColliding(CollisionBox *other) {
    sf::Vector2f topleft = (*other).getPosition() - (*other).getHalfSize();
    sf::Vector2f bottomright = (*other).getPosition() + (*other).getHalfSize();
    
    sf::Vector2f closestPoint = clampVec(getEpicenter(), topleft, bottomright);

    if (getDistance(closestPoint, getEpicenter()) < getRadius()) {
        return true;
        
    } 
    return false;
}

sf::Vector2f CollisionCircle::getEpicenter() {
    return epicenter;
}

float CollisionCircle::getRadius() {
    return radius;
}
