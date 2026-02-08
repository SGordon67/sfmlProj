#include "QuadTree.h"
#include "PhysicalObject.h"

void QuadTree::subdivide()
{
    float subWidth = bounds.size.x / 2.0f;
    float subHeight = bounds.size.y / 2.0f;
    float x = bounds.position.x;
    float y = bounds.position.y;

    nodes[0] = std::make_unique<QuadTree>(level + 1, 
            sf::FloatRect(bounds.position, sf::Vector2f(subWidth, subHeight)));
    nodes[1] = std::make_unique<QuadTree>(level + 1, 
            sf::FloatRect(sf::Vector2f(x + subWidth, y), sf::Vector2f(subWidth, subHeight)));
    nodes[2] = std::make_unique<QuadTree>(level + 1, 
            sf::FloatRect(sf::Vector2f(x, y + subHeight), sf::Vector2f(subWidth, subHeight)));
    nodes[3] = std::make_unique<QuadTree>(level + 1, 
            sf::FloatRect(sf::Vector2f(x + subWidth, y + subHeight), sf::Vector2f(subWidth, subHeight)));
}

int QuadTree::getIndex(const sf::Vector2f& pos, float radius) const
{
    // -1 if it's within one radius of a quadrant boundary
    int index = -1;
    float verticalMidpoint = bounds.position.x + bounds.size.x / 2.0f;
    float horizontalMidpoint = bounds.position.y + bounds.size.y / 2.0f;

    bool topQuadrant = (pos.y - radius < horizontalMidpoint);
    bool bottomQuadrant = (pos.y + radius > horizontalMidpoint);

    if (pos.x + radius < verticalMidpoint)
    {
        if (topQuadrant && !bottomQuadrant)
            index = 0;
        else if (!topQuadrant && bottomQuadrant)
            index = 2;
    } else if (pos.x - radius > verticalMidpoint)
    {
        if (topQuadrant && !bottomQuadrant)
            index = 1;
        else if (!topQuadrant && bottomQuadrant)
            index = 3;
    }
    return index;
}

QuadTree::QuadTree(int level, const sf::FloatRect& bounds) 
    : level(level), bounds(bounds) 
{
}

void QuadTree::retrieveToroidal(std::vector<std::shared_ptr<PhysicalObject>>& returnObjects,
        const sf::Vector2f& pos, float radius)
{
    // avoid duplicates with unordered set
    std::unordered_set<std::shared_ptr<PhysicalObject>> uniqueObjects;
    std::vector<std::shared_ptr<PhysicalObject>> tempResults;

    // get objects close to the real position
    retrieve(tempResults, pos, radius);
    for(const auto& obj : tempResults)
        uniqueObjects.insert(obj);

    // get objects close to the borders near the ghost positions
    auto wrappedPositions = getDupPositions(pos, radius);
    for(const auto& wrappedPos : wrappedPositions)
    {
        tempResults.clear();
        retrieve(tempResults, wrappedPos, radius);
        for(const auto& obj : tempResults)
            uniqueObjects.insert(obj);
    }

    returnObjects.clear();
    returnObjects.reserve(uniqueObjects.size());
    for(const auto& obj : uniqueObjects)
    {
        returnObjects.push_back(obj);
    }
}

void QuadTree::retrieveEntities(std::vector<Entity*>& entities,
        const sf::Vector2f& pos, float radius)
{
    std::vector<std::shared_ptr<PhysicalObject>> allObjects;
    retrieveToroidal(allObjects, pos, radius);

    entities.clear();
    for(auto& obj : allObjects)
    {
        auto entity = std::dynamic_pointer_cast<Entity>(obj);
        if(entity)
        {
            entities.push_back(entity.get());
        }
    }
}

void QuadTree::clear()
{
    objects.clear();
    for (int i = 0; i < 4; i++)
    {
        if (nodes[i])
        {
            nodes[i]->clear();
            nodes[i].reset();
        }
    }
}

int QuadTree::getTotalNodes() const
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

int QuadTree::getMaxDepth() const
{
    // if there are no sub-levels
    if(!nodes[0])
        return level;

    // check each sublevel
    int maxDepth = level;
    for(int i = 0; i < 4; i++)
    {
        maxDepth = std::max(maxDepth, nodes[i]->getMaxDepth());
    }
    return maxDepth;
}

void QuadTree::insert(std::shared_ptr<PhysicalObject> obj) {
    // if this level has sublevels
    if (nodes[0])
    {
        // get the quadrant it belongs in and insert
        int index = getIndex(obj->getPosition(), obj->getRadius());
        if (index != -1)
        {
            nodes[index]->insert(obj);
            return;
        }
    }

    // if this level does not have sub-levels,
    // insert then divide if overflowing
    objects.push_back(obj);

    if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS)
    {
        if (!nodes[0])
        {
            subdivide();
        }

        auto it = objects.begin();
        while (it != objects.end())
        {
            int index = getIndex((*it)->getPosition(), (*it)->getRadius());
            if (index != -1)
            {
                nodes[index]->insert(*it);
                it = objects.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

// TODO inbetween four quads implemented, need to add wrapping
void QuadTree::retrieve(std::vector<std::shared_ptr<PhysicalObject>>& returnObjects,
        const sf::Vector2f& pos, float radius)
{
    // get the quadrant it belongs in, or skip if deepest level
    int index = getIndex(pos, radius);
    if (index != -1 && nodes[0]) // not along a boundry and have sublevels
    {
        nodes[index]->retrieve(returnObjects, pos, radius);
    }

    // add all objects on the same level that couldn't be placed into child quadrant
    for (const auto& obj : objects)
    {
        returnObjects.push_back(obj);
    }

    // if the object in inbetween quadrants, check each possible scenario
    if (index == -1 && nodes[0])
    {
        float verticalMidpoint = bounds.position.x + bounds.size.x / 2.0f;
        float horizontalMidpoint = bounds.position.y + bounds.size.y / 2.0f;
        bool spansVertical = (pos.x - radius < verticalMidpoint && pos.x + radius > verticalMidpoint);
        bool spansHorizontal = (pos.y - radius < horizontalMidpoint && pos.y + radius > horizontalMidpoint);
        bool topQuadrant = (pos.y - radius < horizontalMidpoint);
        bool bottomQuadrant = (pos.y + radius > horizontalMidpoint);

        // centered on all four quadrants
        if(spansVertical && spansHorizontal)
        {
            for(int i = 0; i < 4; i++)
                nodes[i]->retrieve(returnObjects, pos, radius);
        }
        else if(spansVertical) // top 2 or bottom 2
        {
            if(topQuadrant && !bottomQuadrant)
            {
                nodes[0]->retrieve(returnObjects, pos, radius);
                nodes[1]->retrieve(returnObjects, pos, radius);
            }
            else
            {
                nodes[2]->retrieve(returnObjects, pos, radius);
                nodes[3]->retrieve(returnObjects, pos, radius);
            }
        }
        else if(spansHorizontal) // left 2 or right 2
        {
            if(pos.x + radius < verticalMidpoint)
            {
                nodes[0]->retrieve(returnObjects, pos, radius);
                nodes[2]->retrieve(returnObjects, pos, radius);
            }
            else
            {
                nodes[1]->retrieve(returnObjects, pos, radius);
                nodes[3]->retrieve(returnObjects, pos, radius);
            }
        }
    }
}
