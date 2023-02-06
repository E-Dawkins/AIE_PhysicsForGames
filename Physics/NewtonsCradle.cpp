#include "NewtonsCradle.h"

void NewtonsCradle::Startup()
{
	Plane* plane = new Plane(glm::vec2(1, 0), -60, glm::vec4(1));
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -60, glm::vec4(1));
	
	Circle* circle = new Circle(glm::vec2(-50, 0), glm::vec2(50, 0), 1.f, 4, glm::vec4(1));
	Circle* circle1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 3.f, 6, glm::vec4(1, 1, 0, 1));
	Circle* circle2 = new Circle(glm::vec2(0), glm::vec2(0), 1.f, 4, glm::vec4(1));
	Circle* circle3 = new Circle(glm::vec2(20, 0), glm::vec2(0), 1.f, 4, glm::vec4(1));
	Circle* circle4 = new Circle(glm::vec2(40, 0), glm::vec2(0), 1.f, 4, glm::vec4(1));
	
	AddActor(plane);
	AddActor(plane2);
	AddActor(circle);
	AddActor(circle1);
	AddActor(circle2);
	AddActor(circle3);
	AddActor(circle4);
}	