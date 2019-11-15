#pragma once
#include "Rigidbody.h"
#include <Gizmos.h>


class AABB : public Rigidbody
{
public:
    AABB(glm::vec2 position, glm::vec2 dimensions, glm::vec2 velocity, float mass, glm::vec4 colour, float linearDrag, float angularDrag, float restitution);
    virtual ~AABB();

    glm::vec2 max;  // are the max x, y cordinates of the shape
    glm::vec2 min; // are the min x, y cordinates of the shape

    virtual void makeGizmo(); // used to place the object in a list to be drawn

    void setMinMax(); //  set the min max values for the extents


    // get set functions
    glm::vec2 getDimensions() { return m_dimensions; }
    glm::vec4 getColour() { return m_colour; }
    void setPosition(glm::vec2 pos) { m_position = pos; }

protected:
    glm::vec2 m_dimensions; // the height and width radius
    glm::vec4 m_colour;

    
};

