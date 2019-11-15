#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include"PhysicsScene.h"

class _2D_Physics_EngineApp : public aie::Application {
public:

	_2D_Physics_EngineApp();
	virtual ~_2D_Physics_EngineApp();

	virtual bool startup(); // called when the app starts
	virtual void shutdown(); // called when the app ends

	virtual void update(float deltaTime); // called every frame
	virtual void draw(); // called every frame and draws all physics objects to the screen

    void setupConinuousDemo(glm::vec2 initPos, glm::vec2 initVElocity, float gravity); // uses kinematic equations top draw a projection of a path

protected:

	aie::Renderer2D*	m_2dRenderer; // is how you draw to screen
	aie::Font*			m_font; // handles font
    PhysicsScene*       m_physicsScene; // holds and haldes objects in the scene
    std::vector<Rigidbody*> m_balls; // holds sphere class objects for later refrence
    float m_timer1; // a timer used to creating and deleting objetcs
};