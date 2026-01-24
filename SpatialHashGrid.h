#ifndef SPATIALHASHGRID_H
#define SPATIALHASHGRID_H

#include <unordered_map>
#include <vector>
#include <memory>
#include "PhysicalObject.h"

class SpatialHashGrid
{
    private:
        float m_cellSize;
        std::unordered_map<long long, std::vector<std::shared_ptr<PhysicalObject>>> m_grid;

        // hash function to convert gric coordinates to a unique key
        long long getKey(int x, int y) const
        {
            return (static_cast<long long>(x) << 32) | (y & 0xFFFFFFFF);
        }

        // convert world position to grid coordinates
        void getGridCoords(const sf::Vector2f& pos, int& x, int& y) const
        {
            x = static_cast<int>(std::floor(pos.x / m_cellSize));
            y = static_cast<int>(std::floor(pos.y / m_cellSize));
        }


    public:
        SpatialHashGrid(float cellSize =  100.0f)
            : m_cellSize(cellSize)
        {
        }

        void clear()
        {
            m_grid.clear();
        }

        void insert(std::shared_ptr<PhysicalObject> obj)
        {
            int x;
            int y;
            getGridCoords(obj->getPosition(), x, y);
            m_grid[getKey(x, y)].push_back(obj);
        }

        void getNearby(const std::shared_ptr<PhysicalObject> obj, std::vector<std::shared_ptr<PhysicalObject>>& nearby) const
        {
            nearby.clear();
            int centerX;
            int centerY;
            getGridCoords(obj->getPosition(), centerX, centerY);

            // check 3x3 grid of cells around the object
            for(int dx = -1; dx <=1; ++dx)
            {
                for(int dy = -1; dy <=1; ++dy)
                {
                    long long key = getKey(centerX + dx, centerY + dy);
                    auto it = m_grid.find(key);
                    if(it != m_grid.end())
                    {
                        for(const auto& physObj : it->second)
                            nearby.push_back(physObj);

                        // nearby.insert(nearby.end(), it->second.begin(), it->second.end());
                    }
                }
            }
        }
};

#endif
