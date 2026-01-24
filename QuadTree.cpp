#include "QuadTree.h"
#include "PhysicalObject.h"

void QuadTree::insert(std::shared_ptr<PhysicalObject> obj) {
    if (nodes[0]) {
        int index = getIndex(obj->getPosition(), obj->getRadius());
        if (index != -1) {
            nodes[index]->insert(obj);
            return;
        }
    }
    
    objects.push_back(obj);
    
    if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
        if (!nodes[0]) {
            subdivide();
        }
        
        auto it = objects.begin();
        while (it != objects.end()) {
            int index = getIndex((*it)->getPosition(), (*it)->getRadius());
            if (index != -1) {
                nodes[index]->insert(*it);
                it = objects.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void QuadTree::retrieve(std::vector<std::shared_ptr<PhysicalObject>>& returnObjects,
                       const sf::Vector2f& pos, float radius) {
    int index = getIndex(pos, radius);
    if (index != -1 && nodes[0]) {
        nodes[index]->retrieve(returnObjects, pos, radius);
    }
    
    for (const auto& obj : objects) {
        returnObjects.push_back(obj);
    }
    
    if (index == -1 && nodes[0]) {
        for (int i = 0; i < 4; i++) {
            nodes[i]->retrieve(returnObjects, pos, radius);
        }
    }
}
