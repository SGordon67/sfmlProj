#ifndef QUADTREE_H
#define QUADTREE_H

#include "BasicObject.h"
#include "Entity.h"
#include "SFML/System/Vector2.hpp"
#include <unordered_set>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class PhysicalObject;

class QuadTree {
private:
    static constexpr int MAX_OBJECTS = 4;  // Max objects before subdividing
    static constexpr int MAX_LEVELS = 10;    // Max subdivision depth

    int level;
    std::vector<std::shared_ptr<PhysicalObject>> objects;
    sf::FloatRect bounds;
    std::unique_ptr<QuadTree> nodes[4];  // NW, NE, SW, SE quadrants

    void subdivide();

    int getIndex(const sf::Vector2f& pos, float radius) const;

public:
    QuadTree(int level, const sf::FloatRect& bounds);

    void clear();

    int getTotalNodes() const;

    int getMaxDepth() const;

    void insert(std::shared_ptr<PhysicalObject> obj);

    void retrieve(std::vector<std::shared_ptr<PhysicalObject>>& returnObjects, 
                  const sf::Vector2f& pos, float radius);

    void retrieveToroidal(std::vector<std::shared_ptr<PhysicalObject>>& returnObjects,
            const sf::Vector2f& pos, float radius);

    void retrieveEntities(std::vector<Entity*>& entities,
            const sf::Vector2f& pos, float radius);
};

#endif
