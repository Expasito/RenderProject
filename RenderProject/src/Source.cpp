
#include <Render/Render.h>
/*
*  Use WASD to move, mouse to look and B to change wireframe mode
*/

int main() {
	srand(time(0));
	Render::init();	
	Render::camera.baseSpeed = 50;
	

	Render::addModel("assets/cube2.obj", "Cube");
	Render::addModel("assets/monkey.obj", "Monkey");


	for (int i = 0; i < 1000; i++) {
		Render::addInstance("Cube",
			{ (float)rand() / (float)RAND_MAX * 100 - 50 ,(float)rand() / (float)RAND_MAX * 100 - 50 ,(float)rand() / (float)RAND_MAX * 100 - 50 },
			//{0,0,0},
			{ (float)rand() / (float)RAND_MAX * 5 , (float)rand() / (float)RAND_MAX * 5 , (float)rand() / (float)RAND_MAX * 5 },
			{ (float)rand() / (float)RAND_MAX * 5 - 2.5 , (float)rand() / (float)RAND_MAX * 5 - 2.5 , (float)rand() / (float)RAND_MAX * 5 - 2.5 }
		);
	}


	Render::removeInstances("Cube");
	Render::removeInstances("Cube");
	Render::removeAllInstances();

	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		//Render::addInstance("Cube", { 1,1,1 }, { 1,1,1 }, { 1,1,1 });

		Render::renderAll();


		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		float milis = (end - begin).count() / 1000000.0;
		std::cout << "Time difference = " << milis << "[ms]" << " FPS: " << 1000.0/milis << "\n";
	}

	Render::exit();



	return 0;
}