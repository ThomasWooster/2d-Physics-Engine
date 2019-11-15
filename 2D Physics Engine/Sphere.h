#pragma once
#include "Rigidbody.h"
#include <Gizmos.h>

class Sphere : public Rigidbody
{
public:
    Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, float linearDrag, float angularDrag, float restitution);
    virtual ~Sphere();

    virtual void makeGizmo(); // used to place the object in a list to be drawn

    float getRadius() { return m_radius; } // returns the radius of the sphere
    glm::vec4 getColour() { return m_colour; } // returns the colour of the sphere

protected:
    float m_radius; // also used to determin the ammount of segments
    glm::vec4 m_colour;
};

