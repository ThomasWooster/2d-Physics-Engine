#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include <glm/ext.hpp>


class Plane : public PhysicsObject
{
public:

    Plane();
    Plane(glm::vec2 normal, float distance);
    virtual ~Plane();

    virtual void fixedUpdate(glm::vec2 gravity, float timeStep); // not usable just here because the physics object requires that a child has this function
    virtual void debug() {} // can be used for error handeling
    virtual void makeGizmo();// used to place the object in a list to be drawn
    virtual void resolveCollision(Rigidbody* actor2, glm::vec2 contact);


    // get functions
    glm::vec2 getNormal() { return m_normal; }
    float getDistanceToOrigin() { return m_distanceToOrigin; }

protected:
    glm::vec2 m_normal; // vector that has a magnitude of 1 and is used for directions
    float m_distanceToOrigin; // distance of the plane from the 0,0 location
};

