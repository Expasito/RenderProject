
#include <Render/Render.h>
#include <chrono>


/*
*  Use WASD to move, mouse to look and B to change wireframe mode
*/


#include <Render/utils/FillerArray.h>

float test_ = 0.0;


//#define DEBUG 0

// I want to test passing function pointers for a generic hashtable hashing algorithm

/*
* The hash function must take in two parameters. The first is of type T, the second is a size parameter
*/
template <typename T>
class HashTable {
public:
	int size, elements;
	int (*hash)(T, int);
	T* arr;

	HashTable(int size_, int (*hash_)(T,int)) {
		size = size_;
		elements = 0;
		arr = new T(size_);
		hash = hash_;

	};

	void add(T obj) {
		int index = hash(obj, size);
		arr[index] = obj;
		elements++;
	}

	void see() {
		std::cout << "Size: " << size << "\n";
		std::cout << "Elements: " << elements << "\n";
		for (int i = 0; i < size; i++) {
			std::cout << arr[i] << "\n";
		}
	}
	~HashTable() {
		delete arr;

	}
private:

};

int hash(int val, int size) {
	return val % size;
}

int main() {

	//HashTable<int> ht(7, &hash);

	//ht.add(1);

	//ht.see();



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
	Render::init(800,800,false);
	Render::camera.baseSpeed = 10;

	Render::loadSave("saves/input.rpo");

	//exit(1);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	

	//Render::addModel("assets/sphere.obj", "Cube",100,100);
	//Render::addModel("assets/sphere.obj", "Cube");
	//Render::addModel("assets/monkey.obj", "Monkey");

	Render::addModel("assets/room.obj", "Room", 100, 100);

	Render::addInstance("Room", { 20,0,0 }, { 0,0,0 }, { 1,1,1 }, { .5,.5,.5 });


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

	//long key = Render::addInstance("Cube", { 110,0,0 }, { 0,0,0 }, { 1,1,1 }, { .75,.75,.01 });

	//Render::addInstance("Cube", { 10,0,0 }, { 0,0,0 }, { 1,1,1 }, { 0,.75,0 });

	//Render::addInstance("Cube",	{ 20,0,0 }, { 0,0,0 }, { 1,1,1 }, { .75,0,0 });


	//Render::removeInstances("Cube");
	//Render::removeInstances("Cube");
	//Render::addInstance("Monkey", {1,1,1}, {90,1,1}, {2,1,4});
	//Render::removeAllInstances();

	//std::vector<float> time(10000);

	int index_ = 14;

	float milis=0;
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

	//for (int i = 0; i < 100; i++) {
	//	Render::addInstance("Cube",
	//		//{ index_ / 10.0,0,0 },
	//		{ (rand() - RAND_MAX / 2) * dist,(rand() - RAND_MAX / 2) * dist ,(rand() - RAND_MAX / 2) * dist },
	//		{ 0,0,0 },
	//		{ 1,1,1 },
	//		{ rand() / (float)RAND_MAX,rand() / (float)RAND_MAX,rand() / (float)RAND_MAX }
	//	);
	//}

	int F = 0;
	int T = 0;

	unsigned int debug;
	glGenBuffers(1, &debug);
	glBindBuffer(GL_ARRAY_BUFFER, debug);
	float height = -1;
	float data[] = {
		-.95,-1,0,
		-.95,height,0,
		-.9,-1,0,
		-.95,height,0,
		-.9,height,0,
		-.9,-1,0

	};
		
	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		glClearColor(0, 0, 0, 1);

		data[4] = height;
		data[10] = height;
		data[13] = height;
		height = (float)milis / 100;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(Render::program2);
		glBindBuffer(GL_ARRAY_BUFFER, debug);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		glUseProgram(Render::program1);
		float currentFrame = glfwGetTime();
		Render::dt = currentFrame - Render::lastFrame;
		Render::lastFrame = currentFrame;
		Render::camera.speed = Render::camera.baseSpeed * Render::dt;
		glUniformMatrix4fv(glGetUniformLocation(Render::program1, "model"), 1, GL_FALSE, glm::value_ptr(Render::model));
		glUniformMatrix4fv(glGetUniformLocation(Render::program1, "view"), 1, GL_FALSE, glm::value_ptr(Render::view));
		glUniformMatrix4fv(glGetUniformLocation(Render::program1, "projection"), 1, GL_FALSE, glm::value_ptr(Render::projection));
		glUniform3fv(glGetUniformLocation(Render::program1, "camPos"), 1, glm::value_ptr(Render::camera.cameraPos));
		glUniform3fv(glGetUniformLocation(Render::program1, "camFront"), 1, glm::value_ptr(Render::camera.cameraFront));

		Render::camera.translate(Render::left, Render::right, Render::up, Render::down, Render::forward, Render::backward);
		Render::view = glm::lookAt(Render::camera.cameraPos, Render::camera.cameraPos + Render::camera.cameraFront, Render::camera.cameraUp);
		Render::projection = glm::perspective(glm::radians(Render::camera.fov), (float)(800.0 / 800.0), .01f, 1000.0f);

		Render::draw();
		for(int i=0;i<100;i++)
			Render::addInstance("Room", { 0,0,0 }, { 1,1,1 }, { 1,1,1 }, { 1,0,1 });


		glfwSwapBuffers(Render::window);
		glfwPollEvents();
		std::cout << "hello\n";
		Render::keepWindow = !glfwWindowShouldClose(Render::window);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		milis = (end - begin).count() / 1000000.0;
		//time.push_back(milis);
		std::cout << "Total Time difference = " << milis << "[ms]" << " FPS: " << 1000.0 / milis << "\n";

	}

	

	//exit(1);


	index_ = 0;
	while (Render::keepWindow) {
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();


		// adding objects goes here


		std::chrono::steady_clock::time_point add = std::chrono::steady_clock::now();

		#ifdef DEBUG 
		
		milis = (add - begin).count() / 1000000.0;
		std::cout << "Add time: " << milis << "[ms]\n";
		add = std::chrono::steady_clock::now();

		//std::cout << "Elements: " << Render::objects[0]->insts->da->elements << "\n";

		#endif

		// F will save the data
		if (F != Render::getKey(GLFW_KEY_F) && F == 0) {
			Render::createSave("saves/input.rpo");
		}
		F = Render::getKey(GLFW_KEY_F);

		// B goes into wireframe mode

		// T will add an instance at the current position
		if (T != Render::getKey(GLFW_KEY_T) && T == 0) {
			
			Render::addInstance("Cube", { -Render::camera.cameraPos.x,Render::camera.cameraPos.y,Render::camera.cameraPos.z }, { 0,0,0 }, { 1,1,1 }, { 1,1,1 });
		}
		T = Render::getKey(GLFW_KEY_T);

		if (Render::getKey(GLFW_KEY_LEFT_CONTROL)) {
			Render::camera.baseSpeed = 50;
		}
		else {
			Render::camera.baseSpeed = 10;
		}




	/*
	* 
	* 
	*  This is where the rendering code ends
	* 
	*/

		//Render::renderAll();



		#ifdef DEBUG

		std::chrono::steady_clock::time_point rend = std::chrono::steady_clock::now();
		milis = (rend - add).count() / 1000000.0;
		std::cout << "Render*: " << milis << "[ms]\n";

		#endif


		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		milis = (end - begin).count() / 1000000.0;
		//time.push_back(milis);
		std::cout << "Total Time difference = " << milis << "[ms]" << " FPS: " << 1000.0/milis << "\n";
		
		//std::cout << "\n";

	}

	Render::exit();



	return 0;
}