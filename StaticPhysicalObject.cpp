#include "StaticPhysicalObject.h"

StaticPhysicalObject::StaticPhysicalObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename)
	: VisualObject(position, size, rotation, renderLayer, filename)
{
	// std::cout << "StaticPhysicalObject constructed" << std::endl;
}
