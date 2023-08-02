
#include <Render/Render.h>
#include <chrono>


/*
*  Use WASD to move, mouse to look and B to change wireframe mode
*/


#include <Render/utils/FillerArray.h>

float test_ = 0.0;


#define DEBUG 0

int main() {



	/*
	* Look into SSBOs for variable length buffers for lights, triangles, etc.
	* Can be used for shadows and ray casting
	*/
	/*
	* Also, look at making a hash table for adding instances or removing them for models
	* Currently, addInstance and RemoveInstance are O(n) but easily could be O(1) with
	* a hash table
	* 
	* 
	* 
	*/

	srand(time(0));
	Render::init();	
	Render::camera.baseSpeed = 100;

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	

	Render::addModel("assets/sphere.obj", "Cube",100,100);
	//Render::addModel("assets/sphere.obj", "Cube");
	//Render::addModel("assets/monkey.obj", "Monkey");


	//float max_dist = 200;
	//for (int i = 0; i < 50000; i++) {
	//	Render::addInstance("Cube",
	//		{ (float)rand() / (float)RAND_MAX * max_dist - max_dist/2.0 ,(float)rand() / (float)RAND_MAX * max_dist - max_dist / 2.0,(float)rand() / (float)RAND_MAX * max_dist - max_dist / 2.0 },
	//		//{0,0,0},
	//		{ (float)rand() / (float)RAND_MAX * 5 , (float)rand() / (float)RAND_MAX * 5 , (float)rand() / (float)RAND_MAX * 5 },
	//		{ (float)rand() / (float)RAND_MAX * 5 - 2.5 , (float)rand() / (float)RAND_MAX * 5 - 2.5 , (float)rand() / (float)RAND_MAX * 5 - 2.5 },
	//		{.5,.5,.5}
	//		//{(float)rand()/(float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX }
	//	);
	//}

	long key = Render::addInstance("Cube", { 110,0,0 }, { 0,0,0 }, { 1,1,1 }, { .75,.75,.01 });

	Render::addInstance("Cube", { 10,0,0 }, { 0,0,0 }, { 1,1,1 }, { 0,.75,0 });

	Render::addInstance("Cube",	{ 20,0,0 }, { 0,0,0 }, { 1,1,1 }, { .75,0,0 });

	Render::addInstance("Cube",
		{ -10,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,0 }

	);

	Render::addInstance("Cube",
		{ -20,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",
		{ 0,10,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",
		{ 0,20,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,0 }

	);

	Render::addInstance("Cube",
		{ 0,-10,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",
		{ 0,-20,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",
		{ 0,0,10 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",
		{ 0,0,20 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,0 }

	);

	Render::addInstance("Cube",
		{ 0,0,-10 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",
		{ 0,0,-20 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	//Render::removeInstances("Cube");
	//Render::removeInstances("Cube");
	//Render::addInstance("Monkey", {1,1,1}, {90,1,1}, {2,1,4});
	//Render::removeAllInstances();

	//std::vector<float> time(10000);

	int index_ = 14;

	float milis;
	float dist = .01;

	//for (int i = 0; i < 1000000; i++) {
	//	Render::addInstance("Cube", index_,
	//		//{ index_ / 10.0,0,0 },
	//		{(rand()-RAND_MAX/2)/dist,(rand() - RAND_MAX / 2)/dist ,(rand() - RAND_MAX / 2)/dist },
	//		{ 0,0,0 },
	//		{ 1,1,1 },
	//		{ rand()/(float)RAND_MAX,rand() / (float)RAND_MAX,rand() / (float)RAND_MAX }
	//	);
	//	index_++;
	//}





	//exit(1);

	for (int i = 0; i < 1000000; i++) {
		Render::addInstance("Cube",
			//{ index_ / 10.0,0,0 },
			{ (rand() - RAND_MAX / 2) * dist,(rand() - RAND_MAX / 2) * dist ,(rand() - RAND_MAX / 2) * dist },
			{ 0,0,0 },
			{ 1,1,1 },
			{ rand() / (float)RAND_MAX,rand() / (float)RAND_MAX,rand() / (float)RAND_MAX }
		);
	}




	index_ = 0;
	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();


		// adding objects goes here


		std::chrono::steady_clock::time_point add = std::chrono::steady_clock::now();

		#ifdef DEBUG 
		
		milis = (add - begin).count() / 1000000.0;
		std::cout << "Add time: " << milis << "[ms]\n";
		add = std::chrono::steady_clock::now();

		std::cout << "Elements: " << Render::objects[0]->insts->da->elements << "\n";

		#endif
		



	/*
	* 
	* 
	*  This is where the rendering code ends
	* 
	*/

		Render::renderAll();

		#ifdef DEBUG

		std::chrono::steady_clock::time_point rend = std::chrono::steady_clock::now();
		milis = (rend - add).count() / 1000000.0;
		std::cout << "Render*: " << milis << "[ms]\n";

		#endif


		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		milis = (end - begin).count() / 1000000.0;
		//time.push_back(milis);
		std::cout << "Total Time difference = " << milis << "[ms]" << " FPS: " << 1000.0/milis << "\n";
		
		std::cout << "\n";

	}

	Render::exit();



	return 0;
}