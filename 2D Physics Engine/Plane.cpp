#include "Plane.h"
#include <Gizmos.h> 



Plane::Plane() : PhysicsObject(shapes::PLANE)
{
    m_distanceToOrigin = 0;
    m_normal = glm::vec2(0, 1);
}

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(shapes::PLANE)
{
    m_normal = glm::normalize(normal);
    m_distanceToOrigin = distance;
}

Plane::~Plane()
{

}


void Plane::makeGizmo()
{
    float lineSegmentLength = 300;
    glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
        // easy to rotate normal through 90 degrees around z
    glm::vec2 parallel(m_normal.y, -m_normal.x);
    glm::vec4 colour(1, 1, 1, 1);
    glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
    glm::vec2 end = centerPoint - (parallel * lineSegmentLength);

    aie::Gizmos::add2DLine(start, end, colour);
}

void Plane::fixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Plane::resolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
    // the plane is not moving so the velocity is just the other actors velocity 
    glm::vec2 vRel = actor2->getVelocity();
    float e = actor2->getElasticity();
    float j = glm::dot(-(1 + e) * (vRel), m_normal) / (1 / actor2->getMass()); // equation that workout how much energy is produced by the imopact

    glm::vec2 force = m_normal * j; // adds a direction to the energy to give yopu a vector force 

    actor2->applyForce(force, contact - actor2->getPosition());
}
