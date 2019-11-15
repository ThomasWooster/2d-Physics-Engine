#pragma once
#include <glm\ext.hpp>

namespace shapes // the name space is to stop some classes with names
{
    enum ShapeType // the enum holds the id for different shapes
    {
        PLANE = 0,
        SPHERE,
        AABB,
        SHAPE_COUNT
    };

};


class PhysicsObject
{
protected:
	PhysicsObject(shapes::ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0; // function that has to be over written by a cild class
    virtual void debug() = 0;// function that has to be over written by a cild class
    virtual void makeGizmo() = 0;// function that has to be over written by a cild class
    virtual void resetPosition() {}; // here in case a child class needs to overwrite

    // get function
    shapes::ShapeType getShapeId() { return m_shapeID; }

protected:
    shapes::ShapeType m_shapeID; // enum holds the shape type of the object
};