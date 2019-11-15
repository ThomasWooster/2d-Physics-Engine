#include "Rigidbody.h"



Rigidbody::Rigidbody(shapes::ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float linearDrag, float angularDrag, float restitution) : PhysicsObject(shapeID)
{
    m_position = position;
    m_velocity = velocity;
    m_rotation = rotation;
    m_mass = mass;
    m_linearDrag = linearDrag;
    m_angularDrag = angularDrag;
    m_elasticity = restitution;
    m_angularVelocity = 0; // default none
    m_isKinematic = false; // defaule is dynamic object
    
}


Rigidbody::~Rigidbody()
{
}


void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
    if (isKinematic()) // this makes it so that is the object is static dont run the r=et of the location update
    {
        return;
    }
    // remember when applying the force of gravity mass cancels out
    m_velocity += gravity * timeStep; // applys gravity to the velocity
    m_position += m_velocity * timeStep; // updates the position by the velocity or speed
    
    m_velocity -= m_velocity * m_linearDrag * timeStep; // applys drag to the velocity
    m_rotation += m_angularVelocity * timeStep; // updates the rotation with the rotational velocity
    m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep; // applys drag to the rotation

    // following if statements handle the momentum stop so that objects actully come to a rest rather than the number just getting infinitly smaller
    if (glm::length(m_velocity) < 0.01f)
    {
        m_velocity = glm::vec2(0, 0);
    }
    if (glm::abs(m_angularVelocity) < 0.01f)
    {
        m_angularVelocity = 0.0f;
    }
}

void Rigidbody::debug()
{

}

void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos)
{
    glm::vec2 acceleration{ 0, 0 };

    acceleration = force / getMass(); // a = f/m  knimatic equation used to find the acceleration 

    m_velocity += acceleration; // applying acceleration to velocity

    m_angularVelocity += (force.y * pos.x - force.x * pos.y) / (getMoment()); // this figures out how much force is applyed to rotational speed
}


void Rigidbody::resloveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2*collisionNormal)
{
    // find the vector between ther centers, or use the provided direction
    // of force, and make sure it's normalized

    glm::vec2 normal;

    if (collisionNormal != nullptr)
    {
        normal = glm::normalize(*collisionNormal);
    }
    else
    {
        normal = glm::normalize(actor2->getPosition() - m_position);
    }
    
    // get the vector perpendicular to the collision normal
    glm::vec2 perp(normal.y, -normal.x);

    // determine the total velocity of the contact points for the two objects,
    // for both linear and rotational

        // 'r' is the radius from axis to application of force
    float r1 = glm::dot(contact - m_position, -perp);
    float r2 = glm::dot(contact - actor2->m_position, perp);
        // velocity of the contact point on this object
    float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
        // velocity of contact point on actor2
    float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

    if (v1 > v2) // they're moving closer
    {
        // calculate the effective mass at the contact point for each object
        // ie how mutch the contact point will move due to the force applied.
        float mass1 = 1.0f / (1.0f / getMass() + (r1 * r1) / getMoment());
        float mass2 = 1.0f / (1.0f / actor2->getMass() + (r2 * r2) / actor2->getMoment());

        float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;

        glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

        // apply equal and opposite forces
        applyForce(-force, contact - m_position);
        actor2->applyForce(force, contact - actor2->m_position);
    }
}