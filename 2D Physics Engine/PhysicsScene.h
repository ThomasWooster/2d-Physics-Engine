#pragma once
#include "Rigidbody.h"
#include <glm\ext.hpp>
#include <vector>

class PhysicsObject;


class PhysicsScene
{
public:
	PhysicsScene();
	virtual ~PhysicsScene();

	void addActer(PhysicsObject* actor); // takes a physics object and adds it to m_actors
	void removeActor(PhysicsObject* actor); // removes the object sfrom m_actors
	void update(float dt); // called every frame and on every time step calls chexk collision functions
	void updateGizmos(); // runs throughb all the objects and sets thier gizmoes
    void debugScene(); // runs all the objects debug functions

    //set and get
	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

    void checkForCollision(); // checks through allt ghe objects and checks if they are colliding and how

    //these are all the different possible ciollisions
    static bool plane2Plane(PhysicsObject*, PhysicsObject*);
    static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
    static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
    static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);
    static bool AAB2AAB(PhysicsObject*, PhysicsObject*);
    static bool AAB2Plane(PhysicsObject*, PhysicsObject*);
    static bool plane2AAB(PhysicsObject*, PhysicsObject*);
    static bool AAB2Sphere(PhysicsObject*, PhysicsObject*);
    static bool sphere2AAB(PhysicsObject*, PhysicsObject*);

protected:
	glm::vec2 m_gravity;
	float m_timeStep; // this is how often the physics scene updates 
	std::vector<PhysicsObject*> m_actors; // hoilds all the objects in the scene
};

