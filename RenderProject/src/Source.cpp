
#include <Render/Render.h>
/*
*  Use WASD to move, mouse to look and B to change wireframe mode
*/


// we will use a hash table to store the vectors for each model
// there is no remove function because a single model does not take up much space but it may get added 
// in a future update
class HashTable {


public:
	int size;
	// vector of vectors
	std::vector<std::string>** arr;
	HashTable(int size_) {
		size = size_;
		arr = (std::vector<std::string>**)malloc(sizeof(std::vector<std::string>*) * size);
		for (int i = 0; i < size; i++) {
			arr[i] = new std::vector<std::string>;
		}
	}

	HashTable() {
		// default will be 20 models
		size = 20;
		arr = (std::vector<std::string>**)malloc(sizeof(std::vector<std::string>*) * size);
		for (int i = 0; i < size; i++) {
			arr[i] = new std::vector<std::string>;
		}
	}

	// add a string to the hash table
	void add(std::string val) {
		int index = _hash(val);
		arr[index]->push_back(val);
	}


	// see all values at each index
	void see() {
		for (int i = 0; i < size; i++) {
			std::vector<std::string>* temp = arr[i];
			std::cout << i <<"(" << temp->size() << ")" << "\n";
			for (std::string c : *temp) {
				std::cout << "  " << c << "\n";
			}
			std::cout << "\n";
		}
	}



private:

	// honestly works pretty well
	// just take the character value times the position and sum it up
	int _hash(std::string val) {
		int sum = 0;
		for (int i = 0; i < val.length(); i++) {
			sum += val[i]*(i+1);
		}


		return sum % size;

	}
};

int main() {
	HashTable ht(50);


	ht.add("Hello");
	ht.add("Monkey");
	ht.add("Monke");
	ht.add("Monk");
	ht.add("Player");
	ht.add("player");
	ht.add("cube");
	ht.add("CuBe");

	ht.add("one");
	ht.add("two");
	ht.add("three");
	ht.add("23wruwjjflsjf");

	ht.add("beans");
	ht.add("beens");

	ht.see();


	//exit(1);

	/*
	* Look into SSBOs for variable length buffers for lights, triangles, etc.
	* Can be used for shadows and ray casting
	*/
	/*
	* Also, look at making a hash table for adding instances or removing them for models
	* Currently, addInstance and RemoveInstance are O(n) but easily could be O(1) with
	* a hash table
	*/

	srand(time(0));
	Render::init();	
	Render::camera.baseSpeed = 10;
	

	Render::addModel("assets/cube2.obj", "Cube");
	Render::addModel("assets/monkey.obj", "Monkey");


	float max_dist = 200;
	for (int i = 0; i < 50000; i++) {
		Render::addInstance("Cube",
			{ (float)rand() / (float)RAND_MAX * max_dist - max_dist/2.0 ,(float)rand() / (float)RAND_MAX * max_dist - max_dist / 2.0,(float)rand() / (float)RAND_MAX * max_dist - max_dist / 2.0 },
			//{0,0,0},
			{ (float)rand() / (float)RAND_MAX * 5 , (float)rand() / (float)RAND_MAX * 5 , (float)rand() / (float)RAND_MAX * 5 },
			{ (float)rand() / (float)RAND_MAX * 5 - 2.5 , (float)rand() / (float)RAND_MAX * 5 - 2.5 , (float)rand() / (float)RAND_MAX * 5 - 2.5 },
			{.5,.5,.5}
			//{(float)rand()/(float)RAND_MAX,(float)rand() / (float)RAND_MAX,(float)rand() / (float)RAND_MAX }
		);
	}

	Render::addInstance("Cube",
		{ 0,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,.01 }

	);

	Render::addInstance("Cube",
		{ 10,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",
		{ 20,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

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

	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();


		Render::renderAll();


		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		float milis = (end - begin).count() / 1000000.0;
		std::cout << "Time difference = " << milis << "[ms]" << " FPS: " << 1000.0/milis << "\n";
	}

	Render::exit();



	return 0;
}