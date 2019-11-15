#include "Sphere.h"



Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, float linearDrag, float angularDrag, float restitution) : Rigidbody(shapes::SPHERE, position, velocity, 0, mass, linearDrag, angularDrag, restitution)
{
    m_radius = radius;
    m_colour = colour;
    m_moment = 0.5f * m_mass * (m_radius * m_radius); // 2/r^2 is the equation for moment in a circle
}


Sphere::~Sphere()
{
}


void Sphere::makeGizmo()
{
    glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius; // finds the point on the edge of the cirlce for the end of the line

    aie::Gizmos::add2DCircle(m_position, m_radius, m_radius + 10, m_colour); // adds the circle itsself 
    aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1)); // the line used to show rotation
}

