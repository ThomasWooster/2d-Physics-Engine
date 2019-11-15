#pragma once
#include "PhysicsObject.h"
class Rigidbody : public PhysicsObject
{
public:
    Rigidbody(shapes::ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float linearDrag, float angularDrag, float restitution);
    virtual ~Rigidbody();

    virtual void fixedUpdate(glm::vec2 gravity, float timeStep); // called every update and handles gravity, friction, constant rotation and contant velocity 
    virtual void debug(); // can be used for error handleing
    void applyForce(glm::vec2 force, glm::vec2 pos); //used to apply a force to a physics object. pos is point of impact
    void resloveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2*collisionNormal = nullptr); // once a collision has been detected resolvecollisin does all the maths to figure out what happens

    //these are the getters and setters
    glm::vec2 getPosition() { return m_position; } 
    void setPosition(glm::vec2 pos) { m_position = pos; }
    float getRotation() { return m_rotation; }
    glm::vec2 getVelocity() { return m_velocity; }
    float getMass() { return (m_isKinematic) ? INT_MAX : m_mass; }// if the objects is kinematic mass is infinit else return mass
    float getLinearDrag() { return m_linearDrag; }
    float getAngularDrag() { return m_angularDrag; }
    float getElasticity() { return m_elasticity; }
    float getMoment() { return (m_isKinematic) ? INT_MAX : m_moment; }// if the objects is kinematic moment is infinit else return moment
    bool isKinematic() { return m_isKinematic; }
    void setKinematic(bool state) { m_isKinematic = state; }

protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;// must be greater than 0
    float m_rotation; // 2D so we only need a single float to represent our rotation
    float m_linearDrag; // between 0 and 1
    float m_angularDrag; // between 0 and 1
    float m_angularVelocity;
    float m_elasticity; // if greater than 1 system gains energy
    float m_moment; // moment of inertia used in reslovecollision to find the mass at a point
    bool m_isKinematic; // wether the object sis dynamic or static
};

