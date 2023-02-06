#include "Collisions.h"

void Collisions::Startup()
{
    #ifdef CircleToPlane
	SetGravity(glm::vec2(0, -10));

	Circle* ball1 = new Circle(glm::vec2(-40, 50), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(20, 55), glm::vec2(0), 10.0f, 6, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(10, 35), glm::vec2(0), 7.0f, 5, glm::vec4(0, 0, 1, 1));
	Circle* ball4 = new Circle(glm::vec2(-15, 45), glm::vec2(0), 1.0f, 2, glm::vec4(1, 0, 1, 1));
	Plane* plane = new Plane(glm::vec2(0.866, 0.5), -20, glm::vec4(1));
	Plane* plane2 = new Plane(glm::vec2(-0.866, 0.5), -20, glm::vec4(1));

	AddActor(ball1);
	AddActor(ball2);
	AddActor(ball3);
	AddActor(ball4);
	AddActor(plane);
	AddActor(plane2);
#endif

#ifdef FallingOnPlane
	SetGravity(glm::vec2(0, -10));
	
	Plane* plane = new Plane(glm::vec2(0.707, 0.707), -40, glm::vec4(1));
	Plane* plane2 = new Plane(glm::vec2(-0.707, 0.707), -40, glm::vec4(1));
	Circle* circle = new Circle(glm::vec2(-50, 10), glm::vec2(0), 1.f, 1, glm::vec4(1, 0, 0, 1));
	Circle* circle1 = new Circle(glm::vec2(30, 40), glm::vec2(0), 2.f, 5, glm::vec4(1, 0, 1, 1));
	Circle* circle2 = new Circle(glm::vec2(-40, 20), glm::vec2(0), 3.f, 6, glm::vec4(0, 1, 1, 1));
	Box* box = new Box(glm::vec2(50, 10), glm::vec2(0), glm::vec2(4), 1.f, 0.f, glm::vec4(0, 0, 1, 1));
	Box* box1 = new Box(glm::vec2(20, 10), glm::vec2(0), glm::vec2(4), 1.f, 27.f, glm::vec4(0, 0, 1, 1));
	Box* box2 = new Box(glm::vec2(30, 20), glm::vec2(0), glm::vec2(4), 1.f, 54.f, glm::vec4(0, 0, 1, 1));
	Box* box3 = new Box(glm::vec2(10, 0), glm::vec2(0), glm::vec2(4), 1.f, 92.f, glm::vec4(0, 0, 1, 1));

	Box* box4 = new Box(glm::vec2(0, -20), glm::vec2(0), glm::vec2(10), 1.f, 0);
	box4->SetKinematic(true);
	
	AddActor(plane);
	AddActor(plane2);
	AddActor(circle);
	AddActor(circle1);
	AddActor(circle2);
	AddActor(box);
	AddActor(box1);
	AddActor(box2);
	AddActor(box3);
	AddActor(box4);
#endif

#ifdef BounceToStop
	SetGravity(glm::vec2(0, -10));
	
	Plane* plane = new Plane(glm::vec2(0, 1), -40, glm::vec4(1), 0.3f);
	Box* box = new Box(glm::vec2(50, 10), glm::vec2(0), glm::vec2(4), 1.f, 0.f, glm::vec4(0, 1, 0, 1), 0.3f);
	Box* box1 = new Box(glm::vec2(49, 20), glm::vec2(0), glm::vec2(4), 1.f, 0.f, glm::vec4(0, 0, 1, 1), 0.3f);
	Circle* circle = new Circle(glm::vec2(-50, 10), glm::vec2(0), 1.f, 4, glm::vec4(1, 0, 0, 1), 0.3f);
	
	AddActor(plane);
	AddActor(box);
	AddActor(box1);
	AddActor(circle);
#endif

	#ifdef BouncePads
	SetGravity(glm::vec2(0, -10));
	
	Box* box = new Box(glm::vec2(0, -30), glm::vec2(0), glm::vec2(10, 2), 1.f, 0);
	Box* box1 = new Box(glm::vec2(-50, -30), glm::vec2(0), glm::vec2(10, 2), 1.f, 0);
	Box* box2 = new Box(glm::vec2(50, -30), glm::vec2(0), glm::vec2(10, 2), 1.f, 0);

	Circle* circle = new Circle(glm::vec2(0), glm::vec2(0), 4.f, 4.f);
	Circle* circle1 = new Circle(glm::vec2(-50, 0), glm::vec2(0), 4.f, 4.f);
	Circle* circle2 = new Circle(glm::vec2(50, 0), glm::vec2(0), 4.f, 4.f);
	
	box->SetKinematic(true);
	box1->SetKinematic(true);
	box2->SetKinematic(true);
	
	AddActor(box);
	AddActor(box1);
	AddActor(box2);
	AddActor(circle);
	AddActor(circle1);
	AddActor(circle2);
#endif

#ifdef PhysicsBucket
	SetGravity(glm::vec2(0, -15));

	Plane* plane = new Plane(glm::vec2(0.707, 0.707), -40, glm::vec4(1));
	Plane* plane1 = new Plane(glm::vec2(-0.707, 0.707), -40, glm::vec4(1));

	plane->SetElasticity(0.5f);
	plane1->SetElasticity(0.5f);
	
	Box* box = new Box(glm::vec2(0, 40), glm::vec2(0), glm::vec2(3), 1.f, 0);
	Box* box1 = new Box(glm::vec2(-10, 50), glm::vec2(0), glm::vec2(5), 1.f, 0);
	Box* box2 = new Box(glm::vec2(5, 60), glm::vec2(0), glm::vec2(8), 1.f, 0);

	box->SetLinearDrag(0.1f);
	box1->SetLinearDrag(1.f);
	box2->SetLinearDrag(0.4f);

	box->SetAngularDrag(0.5f);

	box->SetElasticity(0.6f);
	box1->SetElasticity(0.7f);
	box2->SetElasticity(0.8f);
	
	Circle* circle = new Circle(glm::vec2(0), glm::vec2(0), 1.f, 0.5f);
	Circle* circle1 = new Circle(glm::vec2(20, 0), glm::vec2(0), 1.f, 0.5f);
	Circle* circle2 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 1.f, 0.5f);
	Circle* circle3 = new Circle(glm::vec2(5, -20), glm::vec2(0), 1.f, 0.5f);
	Circle* circle4 = new Circle(glm::vec2(-5, -20), glm::vec2(0), 1.f, 0.5f);
	Circle* circle5 = new Circle(glm::vec2(10, -30), glm::vec2(0), 1.f, 0.5f);
	Circle* circle6 = new Circle(glm::vec2(-10, -30), glm::vec2(0), 1.f, 0.5f);
	
	circle->SetKinematic(true);
	circle1->SetKinematic(true);
	circle2->SetKinematic(true);
	circle3->SetKinematic(true);
	circle4->SetKinematic(true);
	circle5->SetKinematic(true);
	circle6->SetKinematic(true);

	AddActor(plane);
	AddActor(plane1);
	AddActor(box);
	AddActor(box1);
	AddActor(box2);
	AddActor(circle);
	AddActor(circle1);
	AddActor(circle2);
	AddActor(circle3);
	AddActor(circle4);
	AddActor(circle5);
	AddActor(circle6);
#endif
}
