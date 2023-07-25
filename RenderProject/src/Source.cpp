
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


class MinHeap {
public:
	struct Element {
		int key;
		int value;
	};

	Element* arr;
	int elements;
	int size;

	MinHeap(int size_) {
		arr = (Element*)malloc(sizeof(Element) * size_);
		elements = 1;
		size = size_;

		for (int i = 0; i < size; i++) {
			arr[i] = { 0,0 };
		}

	}

	int parent(int key) {
		return key / 2;
	}

	int left(int key) {
		return key * 2;
	}

	int right(int key) {
		return key * 2 + 1;
	}

	int compare(int key, int key2) {
		return key < key2;
	}

	void add(int key, int value) {
		std::cout << "key added: " << key << "\n";
		arr[elements] = { key,value };
		bubbleup(elements);
		elements++;
		//std::cout << "see here\n";
		see();
		//std::cout << "end see\n";
	}

	Element remove() {
		Element out = arr[1];
		//std::cout << "key: " << arr[elements - 1].key << "\n";
		arr[1] = arr[elements-1];
		//std::cout << elements << "\n";
		//std::cout << "herererererer\n";
		//std::cout << "sfjsfjskfjs;lfsf\n";
		bubbledown(1);
		elements--;
		see();
		return out;
	}
	Element remove(int key) {

	}

	void bubbledown(int index) {
		while (right(index) < elements) {
			int leftkey = arr[left(index)].key;
			int rightkey = arr[right(index)].key;
			int larger = leftkey > rightkey ? rightkey : leftkey;
			int dir = leftkey > rightkey ? right(index) : left(index);
			//std::cout << "   index: " << index << " new: " << dir << "\n";
			//std::cout << "   indexK: " << arr[index].key << " newK: " << arr[dir].key << "\n";
			if (larger < arr[index].key) {
				// flip the two and repeat
				Element temp = arr[dir];
				arr[dir] = arr[index];
				arr[index] = temp;
				index = dir;
			}
			else {
				break;
			}
			//std::cout << "seeing now\n";
			//see();
			//std::cout << "stop seeing\n";

		}

	}

	void bubbleup(int index) {
		//std::cout << "         indexes:(Parent,index) " << parent(index) << " " << index << "\n";
		//std::cout << "            keys:{Parent,index) " << arr[parent(index)].key << " " << arr[index].key << "\n";
		if (index > 1 && arr[parent(index)].key > arr[index].key) {
			//std::cout << "     flipping\n";
			// flip the two elements
			Element temp = arr[parent(index)];
			arr[parent(index)] = arr[index];
			arr[index] = temp;
			//index = parent(index);
			bubbleup(parent(index));
		}
	}

	void see() {
		std::cout << "list: ";
		for (int i = 1; i < elements; i++) {
			std::cout << arr[i].key << " ";
		}
		std::cout << "\n";
	}

	



private:


};



/*
* FillerArray is similar to a dynamic array, but it has 2 new features.
* 1. when removing an element, the last element goes in to fill its spot, so all elements are sequentail
* 2. there is a hash table for finding the index of elements efficiently when they get moved.
* Goal time complexity:
* Add: O(1): just add to the end of the array
* Get: O(1): get the index from the hash table and then from the array
* Remove: O(1): remove the element, fill with the last element and update hashtable
*
* All elements are sequential so we do not need to put the correct elements in a new array for graphics
*/
class FillerArray {
public:

	// this is each element in a FillerArray
	struct Element {
		int key;
		//data goes bellow
	};

	// this will hold the keys and indexes
	// Note:  a returned Element object with index of -1 means the get/remove function failed to find since index of -1 is not legal
	class HashTable {
	public:

		struct Element {
			bool used;
			int key;
			int index;
		};

		Element* arr;
		int size;
		HashTable(int size_) {
			arr = (Element*)malloc(sizeof(Element) * size_);
			size = size_;

			init();

		}

		void see() {
			for (int i = 0; i < size; i++) {
				Element temp = arr[i];
				if (temp.used == 1) {
					std::cout << i << ": (" << temp.key << ", " << temp.index << ")\n";
				}
				else {
					std::cout << i << ":\n";

				}
				
			}
			std::cout << "\n";
		}

		void add(int key, int index) {
			int index_ = _hash(key);

			while (true) {
				Element temp = arr[index_];
				// 1 means used
				if (temp.used == 0) {
					//arr[index_] = { 1,key,index };
					arr[index_].used = 1;
					arr[index_].key = key;
					arr[index_].index = index;
					break;
				}
				else {
					index_++;
					if (index_ > size) {
						index_ = 0;
					}
				}
			}
		}

		Element get(int key) {
			int index_ = _hash(key);
			int index__ = index_;

			while (true) {
				Element temp = arr[index_];
				if (temp.key == key) {
					//arr[index_] = { 1,key,index };
					return temp;
					break;
				}
				else {
					index_++;
					if (index_ > size) {
						index_ = 0;
					}
					// if we do not get a match
					if (index_ == index__) {
						return {0,-1,-1};
					}
				}
			}
		}

		Element remove(int key) {
			int index_ = _hash(key);
			int index__ = index_;

			while (true) {
				Element temp = arr[index_];
				if (temp.key == key) {
					//arr[index_] = { 1,key,index };
					arr[index_].used = 0;
					return temp;
					break;
				}
				else {
					index_++;
					if (index_ > size) {
						index_ = 0;
					}
					// if we do not get a match
					if (index_ == index__) {
						return { 0,-1,-1 };
					}
				}
			}
		}

		int _hash(int key) {
			return key % size;
		}

	private:

		void init() {
			for (int i = 0; i < size; i++) {
				arr[i].key = -1;
				arr[i].index = -1;
				arr[i].used = 0;
			}
		}

	};

	// this will hold the actual elements, do the resizing, etc
	class DynamicArray {
	public:

	private:
	};

private:

};


//#include <Render/Instances.h>
int main() {

	FillerArray::HashTable ht(10);

	ht.add(1, 5);

	ht.add(1, 6);

	ht.add(2, 5);

	ht.add(5, 2);
	ht.add(7, 3);

	ht.see();

	FillerArray::HashTable::Element e = ht.remove(1);
	ht.remove(2);
	ht.add(2, 3);

	ht.see();

	exit(1);


	// LOOK INTO USING A BINARY HEAP
	// all elements are there in the array sequentially
	// get, set, remove and add are logn
	// https://en.wikipedia.org/wiki/Binary_heap#:~:text=The%20number%20of%20operations%20required,complexity%20of%20O(1).

	//Instances insts(5);
	//int index__ = 0;
	//while (true) {
	//	insts.add(index__, { index__,0,0 }, {}, {}, {});
	//	//std::cout << insts.table->size << "\n";
	//}
	//exit(1);


	/*
	* 
	* Here is the plan
	* We have a hash table for the instances that takes in a struct which holds all of the data
	* The table will not use chaining but sequential moving to the right for a new spot
	* 
	* Add will be O(1), returns a key( aka an 'index') for hashing
	* get will be O(1), returns a pointer
	* remove will be O(1), return nothing because we delete the information at the location
	* 
	* for rendering
	* we iterate through the hashtable. If the keep variable is true, then add to a list 
	* if keep is false, skip over.
	* Pass this list to the gpu to draw
	* 
	*/



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
	*
	* *
	* Idea2: hash tabel, don't use chaning
	* make it rehash everything if the load is over .75 or something
	* will be one fat O(n) operation but will be once in a while, should be one dropped frame
	* other than that, always o(1)
	* 
	
	
	
	/

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

	Render::addInstance("Cube",1,
		{ 0,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,.01 }

	);

	Render::addInstance("Cube",2,
		{ 10,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",3,
		{ 20,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",4,
		{ -10,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,0 }

	);

	Render::addInstance("Cube",5,
		{ -20,0,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",6,
		{ 0,10,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",7,
		{ 0,20,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,0 }

	);

	Render::addInstance("Cube",8,
		{ 0,-10,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",9,
		{ 0,-20,0 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",10,
		{ 0,0,10 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	Render::addInstance("Cube",11,
		{ 0,0,20 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,.75,0 }

	);

	Render::addInstance("Cube",12,
		{ 0,0,-10 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ 0,.75,0 }

	);

	Render::addInstance("Cube",13,
		{ 0,0,-20 },
		{ 0,0,0 },
		{ 1,1,1 },
		{ .75,0,0 }

	);

	//Render::removeInstances("Cube");
	//Render::removeInstances("Cube");
	//Render::addInstance("Monkey", {1,1,1}, {90,1,1}, {2,1,4});
	//Render::removeAllInstances();

	std::vector<float> time(10000);

	int index_ = 14;


	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		//Render::addInstance("Monkey", { 1,1,1 }, { 90,1,1 }, { 2,1,4 }, {.5,.5,.8});

		Render::addInstance("Cube", index_,
			{ index_/10.0,0,0},
			{ 0,0,0 },
			{ 1,1,1 },
			{ .75,0,0 }
		);
		index_++;



		Render::renderAll();


		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		float milis = (end - begin).count() / 1000000.0;
		time.push_back(milis);
		std::cout << "Time difference = " << milis << "[ms]" << " FPS: " << 1000.0/milis << "\n";
		float sum = 0.0;
		for (int i = 0; i < time.size(); i++) {
			sum += time.at(i);
		}
		std::cout << "Average: " << sum / time.size() << "\n";
	}

	Render::exit();



	return 0;
}