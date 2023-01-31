#include "PhysicsApp.h"

int main() {
	
	// allocation
	const auto app = new PhysicsApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}