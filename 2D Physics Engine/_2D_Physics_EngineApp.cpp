#include "_2D_Physics_EngineApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include <Gizmos.h>
#include <vector>
#include <algorithm>

_2D_Physics_EngineApp::_2D_Physics_EngineApp() {

}

_2D_Physics_EngineApp::~_2D_Physics_EngineApp() {

}

bool _2D_Physics_EngineApp::startup() {
    // increace the 2d line count to maximise the number of objects we can draw
    aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();
	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

    m_physicsScene = new PhysicsScene();
    m_physicsScene->setGravity(glm::vec2(0, -9));
    m_physicsScene->setTimeStep(0.01f);

    //setupConinuousDemo(glm::vec2(-40, 0), glm::vec2(15, 15), -10);

    float radius = 5.0f;
    glm::vec2 startPos(-60, 0);
    glm::vec2 velocity(5, 5);

    Sphere* staticSphere = new Sphere(glm::vec2(-10, 5), glm::vec2(0, 0), 1, 3, glm::vec4(0, 1, 1, 1),0 ,0, 0.9f);
    staticSphere->setKinematic(true);
    m_physicsScene->addActer(staticSphere);

    m_physicsScene->addActer(new Sphere(startPos, velocity, 2, radius, glm::vec4(1, 0, 0, 1), 0.3f, 0.3f, 0.99f));
    m_physicsScene->addActer(new Sphere(glm::vec2(10, 10), glm::vec2(-1, 0), 3, radius, glm::vec4(1, 0, 1, 1), 0.2f, 0.2f, 0.99f));

    m_physicsScene->addActer(new AABB(glm::vec2(0, 15), glm::vec2(3, 3), glm::vec2(0, -4), 1, glm::vec4(1, 0, 0, 1), 0.2f, 0.2f, 0.99f));
    m_physicsScene->addActer(new AABB(glm::vec2(0, 0), glm::vec2(3, 3), glm::vec2(0, 0), 1, glm::vec4(1, 1, 0, 1), 0.2f, 0.2f, 0.99f));


    m_physicsScene->addActer(new Sphere(glm::vec2(-10, 15), glm::vec2(0, -4), 1, 5, glm::vec4(1, 1, 1, 1), 0.2f, 0.2f, 0.99f));

    m_physicsScene->addActer(new Plane(glm::vec2(0.5, 1), -50));
    m_physicsScene->addActer(new Plane(glm::vec2(0, 1), -50));

    m_timer1 = 0.0f;
      
	return true;
}

void _2D_Physics_EngineApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
    delete m_physicsScene;
}

void _2D_Physics_EngineApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();
    aie::Gizmos::clear();

    if (m_timer1 >= 3.0f)
    {
        Sphere* m_temp = new Sphere(glm::vec2(-40, 10), glm::vec2(-1, 0), 3, 2, glm::vec4(1, 0, 0, 1), 0.2f, 0.2f, 0.99f);

        m_balls.push_back(m_temp);
        m_physicsScene->addActer(m_temp);

        // this if state ment creates a newshpere object and adds it to the physics scene 
        //as well as after the first 3 spawnings destroys the oldes shpere every 3 secconds 
        if (m_balls.size() > 3) 
            
        {
            m_physicsScene->removeActor(m_balls[0]);

            m_balls.erase(std::remove(std::begin(m_balls), std::end(m_balls), m_balls[0]), std::end(m_balls));
        }

        m_timer1 = 0.0f;
    }

    m_timer1 += deltaTime;
    //m_actors.erase(std::remove(std::begin(m_actors), std::end(m_actors), actor), std::end(m_actors))

    m_physicsScene->update(deltaTime);
    m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void _2D_Physics_EngineApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
    static float aspectRatio = 16 / 9.0f;
    aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void _2D_Physics_EngineApp::setupConinuousDemo(glm::vec2 initPos, glm::vec2 initVElocity, float gravity)
{
    float t = 0;
    float tStep = 0.5f;
    float radius = 1.0f;
    int segments = 12;
    glm::vec4 colour = glm::vec4(1, 1, 0, 1);

    while (t <= 5)
    {
        // calculate the x, y position of the projectile at time 
       
        float changeInPosY = (initVElocity.y * t) + (0.5f * gravity * (t * t));
        float changeInPosX = initVElocity.x * t;

        aie::Gizmos::add2DCircle(glm::vec2(initPos.x + changeInPosX, initPos.y + changeInPosY), radius, segments, colour);
        t += tStep;
    }
}