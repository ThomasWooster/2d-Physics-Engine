#include "AABB.h"



AABB::AABB(glm::vec2 position, glm::vec2 dimensions, glm::vec2 velocity, float mass, glm::vec4 colour, float linearDrag, float angularDrag, float restitution) : Rigidbody(shapes::AABB, position, velocity, 0, mass, linearDrag, angularDrag, restitution)
{
    m_dimensions = dimensions;
    m_colour = colour;

    
}


AABB::~AABB()
{
}


void AABB::makeGizmo()
{
    aie::Gizmos::add2DAABBFilled(m_position, m_dimensions, m_colour);
}


void AABB::setMinMax()
{
    max = m_position + m_dimensions;
    min = m_position - m_dimensions;
}