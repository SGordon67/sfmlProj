#ifndef QUADTREE_H
#define QUADTREE_H

#include "SFML/System/Vector2.hpp"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class PhysicalObject;

class QuadTree {
private:
    static constexpr int MAX_OBJECTS = 10;  // Max objects before subdividing
    static constexpr int MAX_LEVELS = 5;    // Max subdivision depth

    int level;
    std::vector<std::shared_ptr<PhysicalObject>> objects;
    sf::FloatRect bounds;
    std::unique_ptr<QuadTree> nodes[4];  // NW, NE, SW, SE quadrants

    void subdivide() {
        float subWidth = bounds.size.x / 2.0f;
        float subHeight = bounds.size.y / 2.0f;
        float x = bounds.position.x;
        float y = bounds.position.y;

        nodes[0] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(bounds.position, bounds.size));
        nodes[1] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(sf::Vector2f(x + subWidth, y), bounds.size));
        nodes[2] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(sf::Vector2f(x, y + subHeight), bounds.size));
        nodes[3] = std::make_unique<QuadTree>(level + 1, sf::FloatRect(sf::Vector2f(x + subWidth, y + subHeight), bounds.size));
    }

    int getIndex(const sf::Vector2f& pos, float radius) const {
        int index = -1;
        float verticalMidpoint = bounds.position.x + bounds.size.x / 2.0f;
        float horizontalMidpoint = bounds.position.y + bounds.size.y / 2.0f;

        bool topQuadrant = (pos.y - radius < horizontalMidpoint);
        bool bottomQuadrant = (pos.y + radius > horizontalMidpoint);

        if (pos.x - radius < verticalMidpoint && pos.x + radius < verticalMidpoint) {
            if (topQuadrant && !bottomQuadrant) index = 0;
            else if (!topQuadrant && bottomQuadrant) index = 2;
        } else if (pos.x - radius > verticalMidpoint && pos.x + radius > verticalMidpoint) {
            if (topQuadrant && !bottomQuadrant) index = 1;
            else if (!topQuadrant && bottomQuadrant) index = 3;
        }

        return index;
    }

public:
    QuadTree(int level, const sf::FloatRect& bounds) 
        : level(level), bounds(bounds) {}

    void clear() {
        objects.clear();
        for (int i = 0; i < 4; i++) {
            if (nodes[i]) {
                nodes[i]->clear();
                nodes[i].reset();
            }
        }
    }

    int getTotalNodes() const
    {
        int count = 1;
        if(nodes[0])
        {
            for(int i = 0; i < 4; i++)
            {
                count += nodes[i]->getTotalNodes();
            }
        }
        return count;
    }

    int getMaxDepth() const
    {
        if(!nodes[0]) return level;
        int maxDepth = level;
        for(int i = 0; i < 4; i++)
        {
            maxDepth = std::max(maxDepth, nodes[i]->getMaxDepth());
        }
        return maxDepth;
    }

    void insert(std::shared_ptr<PhysicalObject> obj);

    void retrieve(std::vector<std::shared_ptr<PhysicalObject>>& returnObjects, 
                  const sf::Vector2f& pos, float radius);
};

#endif
