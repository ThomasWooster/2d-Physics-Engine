#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>



PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = { 0, 0 };
}


PhysicsScene::~PhysicsScene()
{
    for (auto pActor : m_actors)
    {
        delete pActor;
    }
}


void PhysicsScene::addActer(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	m_actors.erase(std::remove(std::begin(m_actors), std::end(m_actors), actor), std::end(m_actors));
}

void PhysicsScene::update(float dt)
{
    static std::list<PhysicsObject*> dirty;

    // update the physics at a fixed time step

    static float accumulatedTime = 0.0f;
    accumulatedTime += dt;

    while (accumulatedTime >= m_timeStep)
    {
        for (PhysicsObject* pActor : m_actors)
        {
            pActor->fixedUpdate(m_gravity, m_timeStep);
        }

        accumulatedTime -= m_timeStep;

        checkForCollision();

        // check for collisions (ideally you'd want to have some sort of
        // scene management in place)

        
    }
;}

void PhysicsScene::updateGizmos()
{
    for  (PhysicsObject* pActor : m_actors)
    {
        pActor->makeGizmo();
    }
}

void PhysicsScene::debugScene()
{
    int count = 0;
    for (auto pActor : m_actors)
    {
        std::cout << count << " : ";
        pActor->debug();
        count++;
    }
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
    PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2AAB,
    PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2AAB,
    PhysicsScene::AAB2Plane, PhysicsScene::AAB2Sphere, PhysicsScene::AAB2AAB
    
};

void PhysicsScene::checkForCollision()
{
    int actorCount = m_actors.size();

    // neede to check for collisions with all objects exept for this one
    for (int outer = 0; outer < actorCount - 1; outer++)
    {
        for (int inner = outer + 1; inner < actorCount; inner++)
        {
            PhysicsObject* object1 = m_actors[outer];
            PhysicsObject* object2 = m_actors[inner];
            int shapeId1 = object1->getShapeId();
            int shapeId2 = object2->getShapeId();

            // using functuion pointers
            int functionIdx = (shapeId1 * shapes::SHAPE_COUNT) + shapeId2;
            fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
            if (collisionFunctionPtr != nullptr)
            {
                // did a collision occur?
                collisionFunctionPtr(object1, object2);
            }
        }
    }
}\

bool PhysicsScene::plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
    return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject*obj2)
{
    return sphere2Plane(obj2, obj1);
}

bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
    Sphere* sphere = dynamic_cast<Sphere*>(obj1);
    Plane* plane = dynamic_cast<Plane*>(obj2);

    // if we are sucsessful we then test for collision
    if (sphere != nullptr && plane != nullptr)
    {
        glm::vec2 collisionNormal = plane->getNormal();
        float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistanceToOrigin();

        // if we are behind the plane we flip the normal
        if (sphereToPlane < 0)
        {
            collisionNormal *= -1;
            sphereToPlane *= -1;
        }

        float intersection = sphere->getRadius() - sphereToPlane;
        if (intersection > 0)
        {
            sphere->setPosition(sphere->getPosition() + plane->getNormal() * (sphere->getRadius() - sphereToPlane));

            glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());

            plane->resolveCollision(sphere, contact);

            return true;
        }
    }

    return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
    //try to cast the objects to sphere and sphere
    Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
    Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
    //if we are sucessful then test for collision

    if (sphere1 != nullptr && sphere2 != nullptr)
    {
        glm::vec2 delta = sphere2->getPosition() - sphere1->getPosition();
        float distance = glm::length(delta);
        float intersection = sphere1->getRadius() + sphere2->getRadius() - distance;

        if (intersection > 0)
        {
            glm::vec2 contactForce = 0.5f * (distance - (sphere1->getRadius() + sphere2->getRadius())) * delta / distance;


            if (!sphere1->isKinematic() && !sphere2->isKinematic())
            {
                sphere1->setPosition(sphere1->getPosition() + contactForce);
                sphere2->setPosition(sphere2->getPosition() - contactForce);
            }
            else if (!sphere1->isKinematic())
            {
                sphere1->setPosition(sphere1->getPosition() + contactForce * 2.0f);
            }
            else if (!sphere2->isKinematic())
            {
                sphere2->setPosition(sphere2->getPosition() - contactForce * 2.0f);
            }

            

            //respond to the collision
            sphere1->resloveCollision(sphere2, 0.5f * (sphere1->getPosition() + sphere2->getPosition()));
        }
       
    }

    return false;
}

bool PhysicsScene::AAB2AAB(PhysicsObject* obj1, PhysicsObject* obj2)
{
    AABB* box1 = dynamic_cast<AABB*>(obj1);
    AABB* box2 = dynamic_cast<AABB*>(obj2);

    box1->setMinMax();
    box2->setMinMax();

    if (box1 != nullptr && box2 != nullptr)
    {
        float left = box1->min.x;
        float right = box1->max.x;
        float top = box1->max.y;
        float bottom = box1->min.y;
        if ((bottom <= box2->max.y && bottom >= box2->min.y) || (box1->max.y >= box2->min.y && box1->max.y <= box2->max.y))
        {
            if ((left <= box2->max.x && left >= box2->min.x) || (box1->max.x >= box2->min.x && box1->max.x <= box2->max.x))
            {
                float elasticity = (box1->getElasticity() + box2->getElasticity()) / 2.0f;

                glm::vec2 normal = glm::normalize(box2->getPosition() - box1->getPosition());
                glm::vec2 relativeVelocity = box2->getVelocity() - box1->getVelocity();

                float dot1 = glm::dot(-(1 + elasticity) * (relativeVelocity), normal);
                float dot2 = glm::dot(normal, normal * ((1 / box1->getMass()) + (1 / box2->getMass())));

                float j = dot1 / dot2;

                glm::vec2 force = normal * j;

                box2->setPosition(box2->getPosition() + normal * 0.1f);

                box1->applyForce(-force, box1->getPosition());
                box2->applyForce(force, box2->getPosition());
               // std::cout << "collided" << std::endl;
            }
        }
    }

    return false;
}

bool PhysicsScene::AAB2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
    AABB* box = dynamic_cast<AABB*>(obj1);
    Plane* plane = dynamic_cast<Plane*>(obj2);

    box->setMinMax();

    if (box != nullptr && plane != nullptr)
    {
        glm::vec2 A{ box->min.x, box->max.y };
        glm::vec2 B{ box->max.x, box->max.y };
        glm::vec2 C{ box->max.x, box->min.y };
        glm::vec2 D{ box->min.x, box->min.y }; 

        std::vector<glm::vec2> corners;
        corners.push_back(A);
        corners.push_back(B);
        corners.push_back(C);
        corners.push_back(D);

        float radius;


        if (glm::dot(A, plane->getNormal()) - plane->getDistanceToOrigin() == glm::dot(B, plane->getNormal()) - plane->getDistanceToOrigin())
        {
            radius = box->getDimensions().y;
        }
        else if (glm::dot(B, plane->getNormal()) - plane->getDistanceToOrigin() == glm::dot(C, plane->getNormal()) - plane->getDistanceToOrigin())
        {
            radius = box->getDimensions().y;
        }

        else
        {
            for (int i = 0; i < corners.size() - 1; i++)
            {
                glm::vec2 collisionNormal = plane->getNormal();
                float boxToPlane = glm::dot(box->getPosition(), plane->getNormal()) - plane->getDistanceToOrigin();

                // if we are behind the plane we flip the normal
                if (boxToPlane < 0)
                {
                    collisionNormal *= -1;
                    boxToPlane *= -1;
                }

                radius = glm::distance(corners[i], box->getPosition());

                float intersection = radius - boxToPlane;
                if (intersection > 0)
                {
                    box->setPosition(box->getPosition() + plane->getNormal() * (radius - boxToPlane));

                    float elacticity = 1;
                    float j = glm::dot(-(1 + elacticity) * (box->getVelocity()), collisionNormal) / glm::dot(collisionNormal, collisionNormal * ((1 / box->getMass())));

                    glm::vec2 force = collisionNormal * j;

                    box->applyForce(force, box->getPosition());

                    return true;
                }
            }
        }

        glm::vec2 collisionNormal = plane->getNormal();
        float boxToPlane = glm::dot(box->getPosition(), plane->getNormal()) - plane->getDistanceToOrigin();

        // if we are behind the plane we flip the normal
        if (boxToPlane < 0)
        {
            collisionNormal *= -1;
            boxToPlane *= -1;
        }

        float intersection = radius - boxToPlane;
        if (intersection > 0)
        {
            box->setPosition(box->getPosition() + plane->getNormal() * (radius - boxToPlane));

            float elacticity = 1;
            float j = glm::dot(-(1 + elacticity) * (box->getVelocity()), collisionNormal) / glm::dot(collisionNormal, collisionNormal * ((1 / box->getMass())));

            glm::vec2 force = collisionNormal * j;

            box->applyForce(force, box->getPosition());

            return true;
        }

        
    }

    return false;
}

bool PhysicsScene::plane2AAB(PhysicsObject* obj1, PhysicsObject* obj2)
{
    return false;
}

bool PhysicsScene::AAB2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
    AABB* box = dynamic_cast<AABB*>(obj1);
    Sphere* sphere = dynamic_cast<Sphere*>(obj2);

    box->setMinMax();

    if (box != nullptr && sphere != nullptr)
    {
        glm::vec2 closePointBox;

        glm::vec2 posSphere = sphere->getPosition();

        closePointBox.x = glm::clamp(posSphere.x, box->min.x, box->max.x);
        closePointBox.y = glm::clamp(posSphere.y, box->min.y, box->max.y);

        if (sphere->getRadius() > glm::distance(posSphere, closePointBox))
        {
           box->resloveCollision(sphere, sphere->getPosition());


            return true;
        }
    }

    return false;
}

bool PhysicsScene::sphere2AAB(PhysicsObject* obj1, PhysicsObject* obj2)
{
    return AAB2Sphere(obj2, obj1);
}