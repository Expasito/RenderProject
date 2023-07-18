
#include <Render/Render.h>
#include <chrono>


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
	std::vector<Render::object*>** arr;
	HashTable(int size_) {
		size = size_;
		arr = (std::vector<Render::object*>**)malloc(sizeof(std::vector<Render::object*>*) * size);
		for (int i = 0; i < size; i++) {
			arr[i] = new std::vector<Render::object*>;
		}

	}

	// add a string to the hash table
	void add(Render::object* val) {
		int index = _hash(val->name);
		arr[index]->push_back(val);
	}


	// see all values at each index
	void see() {
		for (int i = 0; i < size; i++) {
			std::vector<Render::object*>* temp = arr[i];
			std::cout << i <<"(" << temp->size() << ")" << "\n";
			for (int j = 0; j < temp->size(); j++) {
				std::cout << (temp)->at(j)->name << "\n";
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





template<typename T>
class HashTable2 {


public:
	int size;
	// vector of vectors
	std::vector<T>** arr;
	HashTable2(int size_) {
		size = size_;
		arr = (std::vector<T>**)malloc(sizeof(std::vector<T>*) * size);
		for (int i = 0; i < size; i++) {
			arr[i] = new std::vector<T>;
		}

	}

	// does not work right now, may make two methods or two hashtables(preferably not)
	void add(T val) {
		// this is the hashcode for a string
		if (typeid(val).hash_code() == 10283618467285603348) {
			int index = _hash(val->name);
			arr[index]->push_back(val);
		}

		// this is for unsigned long long
		if (typeid(val).hash_code() == 1) {
			int index = _hash(val);
			arr[index]->push_back(val);
		}

		
	}


	// see all values at each index
	void see() {
		for (int i = 0; i < size; i++) {
			std::vector<T>* temp = arr[i];
			std::cout << i << "(" << temp->size() << ")" << "\n";
			for (int j = 0; j < temp->size(); j++) {
				std::cout << (temp)->at(j) << "\n";
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
			sum += val[i] * (i + 1);
		}


		return sum % size;

	}

	int _hash(unsigned long long val) {
		return val % size;
	}
};



int main() {



	/*
	* 
	* 
	* Constraints:
	* We have buffers of data for translations, rotations, scalations, colors
	* 
	* Goal:
	* make it so adding, editing and removing an instance is O(1) vs O(n^2)
	* 
	* A hashtable fixes the issue with different render::objects so we are upto O(n) once implemented
	* 
	* We need a way to get it down to constant time for adding an element and being able to edit it
	* Edit could just be use the index but removing an element in the array leads to issues because indexes get messed up
	* 
	* 
	* Idea: we use a hashtable, but rather than chaining, move the index over one. Then we have a buffer of x possible elements
	* and we will get an approximate index. Then move right until we find the exact match of ids and edit there.
	*/

	//HashTable2<unsigned long long> ht2(10);

	//ht2.add(100);
	//ht2.add(20);

	//ht2.see();

	//exit(1);


	// test performance of a collatz conjecture test with heap vs stack

	// stack here
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	long long  x = 99999999999999;

	std::cout << typeid(x).hash_code() << "\n";
	std::cout << typeid(std::string).hash_code() << "\n";
	while (x != 1) {
		if (x % 2 == 0) {
			x /= 2;
		}
		else {
			x = 3 * x + 1;
		}
		//std::cout << x << "\n";
	}

	//long long* x2 = new long long;
	//
	//*x2 = 99999999999999;

	//while (*x2 != 1) {
	//	if (*x2 % 2 == 0) {
	//		*x2 /= 2;
	//	}
	//	else {
	//		*x2 = 3 * *x2 + 1;
	//	}
	//	std::cout << *x2 << "\n";
	//}


	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	float milis = (end - start).count()/1000000.0;
	
	std::cout << milis << "\n";

	//exit(1);
	HashTable ht(5);


	Render::object o = { {},"hello", new std::vector<glm::vec3>, new std::vector<glm::vec3>, new std::vector<glm::vec3>, new std::vector<glm::vec3> };
	ht.add(&o);



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
	* 
	* 
	* 
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

		//Render::addInstance("Monkey", { 1,1,1 }, { 90,1,1 }, { 2,1,4 }, {.5,.5,.8});

		Render::renderAll();


		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		float milis = (end - begin).count() / 1000000.0;
		std::cout << "Time difference = " << milis << "[ms]" << " FPS: " << 1000.0/milis << "\n";
	}

	Render::exit();



	return 0;
}